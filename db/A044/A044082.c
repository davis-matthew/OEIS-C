/*
 * A044082  Numbers n such that string 1,0 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A044082: numbers n whose base‑5 representation contains the
 * substring "10" but that of n-1 does not.
 * The function performs a simple brute‑force search.
 */

#include <stdbool.h>
#include <stdint.h>

/* Return true if the base‑5 representation of x contains the substring "10". */
static bool has10_seq5(seq_t x)
{
    if (x == (seq_t)0)
        return false;

    /* A 128‑digit buffer is far more than enough for the values we generate. */
    uint8_t digits[128];
    int len = 0;

    while (x != (seq_t)0) {
        seq_t q = x / ((seq_t)5);
        seq_t r = x % ((seq_t)5);
        digits[len++] = (uint8_t)r;
        x = q;
    }

    /* Scan the digits from most to least significant. */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 1 && digits[i - 1] == 0)
            return true;
    }
    return false;
}

/* Return the nth term of the sequence (offset = 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* The offset is 1; this value is unused. */

    idx_t count = 0;
    seq_t cand = (seq_t)1;

    for (;; ++cand) {
        if (has10_seq5(cand)) {
            seq_t prev = cand - ((seq_t)1);
            if (!has10_seq5(prev)) {
                ++count;
                if (count == n)
                    return cand;
            }
        }
    }
}
