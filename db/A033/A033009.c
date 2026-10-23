/*
 * A033009  Every run of digits of n in base 11 has length 2.
 * https://oeis.org/A033009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 12*A043315(n) (numbers whose base‑11 representation *
 *            consists of equal‑digit runs of length 2). The algorithm maps
 *            the * index n to the tuple of k pair digits. For n ≤10 the value
 *            is 12*n; * otherwise the number of pairs k is chosen so that *
 *            Σ_{i=1}^{k-1}10^i < n ≤ Σ_{i=1}^{k}10^i. * The...
 * Range:     u8 n <= 10, u16 n <= 110, u32 n <= 1100, u64 n <= 1100,
 *            u128 n <= 1100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n)
{
    /* Handle n=0 defensively */
    if (n == 0)
        return 0;

    /* First 10 terms are simple multiples of 12 */
    if (n <= 10)
        return (seq_t)n * 12;

    /* ---------- Find number of pairs k ---------- */
    idx_t pow10 = 10;          /* 10^(1) */
    idx_t cumulative = 10;     /* Σ_{i=1}^{1}10^i */
    idx_t k = 1;

    while (cumulative < n) {
        if (pow10 > UINT64_MAX / 10)
            break;           /* should not happen for intended limits */
        pow10 *= 10;
        if (cumulative > UINT64_MAX - pow10)
            break;
        cumulative += pow10;
        ++k;
    }

    /* Previous cumulative count (k-1 pairs) */
    idx_t prev_cumulative = cumulative - pow10;
    idx_t offset = n - prev_cumulative - 1;   /* 0‑based within this block */

    /* ---------- Convert offset to base‑10 digits (k digits, leading
     * zeros allowed) ----------
     */
    uint8_t dig[64];   /* k will never exceed 64 for all realistic n */
    idx_t rem = offset;
    for (idx_t i = k; i-- > 0;) {
        dig[i] = rem % 10;
        rem /= 10;
    }

    /* ---------- Build the value in base 11 ----------
     * Each pair digit is repeated twice.
     */
    seq_t val = 0;
    /* First pair */
    seq_t prev_digit = (seq_t)(dig[0] + 1);    /* 1‑10 */
    val = prev_digit;
    val = val * 11 + prev_digit;

    /* Remaining pairs */
    for (idx_t i = 1; i < k; ++i) {
        uint8_t t = dig[i];
        seq_t digit = (seq_t)t;
        if ( (seq_t)t + 1 <= 10 && (seq_t)t >= prev_digit )
            digit = (seq_t)(t + 1);  /* skip previous digit */
        if (t < prev_digit)
            digit = (seq_t)t;       /* keep as is */

        /* It is safe to assign digit as above: digit will be 0‑10 */
        val = val * 11 + digit;
        val = val * 11 + digit;
        prev_digit = digit;
    }

    return val;
}
