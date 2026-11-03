/*
 * A234578  Let S_n = 0 followed by base-3 expansion of n, reversed; sequence
 *          is concatenation of S_0, S_1, S_2, ...
 * https://oeis.org/A234578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th digit (0‑based) in the concatenation of strings
 *            S[i] = 0 followed by the reverse of the ternary representation
 *            of i
 * Range:     u8 n <= 10003, u16 n <= 10003, u32 n <= 10003, u64 n <= 10003,
 *            u128 n <= 10003, big any n
 * Validated: standard, pass, 121 terms through n = 7868 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute Σ_{i=1}^m ⌊log_3(i)⌋ using 3‑power ranges. */
static idx_t sum_floor_u64(idx_t m)
{
    idx_t total = 0;
    idx_t pow   = 1;   /* 3^k */
    idx_t k     = 0;
    while (pow <= m) {
        idx_t upper;
        if (pow > m / 3)                 /* next power would exceed m */
            upper = m;
        else
            upper = pow * 3 - 1;
        idx_t cnt = upper - pow + 1;
        total += cnt * k;

        if (pow > m / 3)                 /* no more ranges */
            break;
        pow *= 3;
        ++k;
    }
    return total;
}

/* Cumulative length up to index m: Σ_{i=0}^m (1 + digits_3(i))  */
static unsigned __int128 cumulative_length128(idx_t m)
{
    idx_t sum_floor = sum_floor_u64(m);
    return (unsigned __int128)m * 2 + 2 + sum_floor;
}

/* Number of ternary digits of n (n ≥ 0).  */
static int digits_base3(idx_t n)
{
    int d = 0;
    do {
        ++d;
        n /= 3;
    } while (n);
    return d;
}

seq_t a(idx_t n)
{
    /* binary search: first m with cumulative_length(m) > n */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 cmid = cumulative_length128(mid);
        if (cmid > (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* previous cumulative length */
    unsigned __int128 prevC;
    if (m == 0)
        prevC = 0;
    else
        prevC = cumulative_length128(m - 1);

    idx_t offset = (idx_t)((unsigned __int128)n - prevC);   /* position inside S_m */
    if (offset == 0)            /* the leading leading 0 */
        return (seq_t)0;

    /* otherwise offset-1 is the index of the desired digit in
       the reversed base‑3 representation of m (least significant first). */
    idx_t index = offset - 1;
    idx_t pow3  = 1;
    for (idx_t i = 0; i < index; ++i)
        pow3 *= 3;

    return (seq_t)((m / pow3) % 3);
}
