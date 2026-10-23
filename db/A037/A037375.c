/*
 * A037375  Positive numbers k such that every base-2 digit of k is a base-6
 *          digit of k.
 * https://oeis.org/A037375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer k such that every base‑2 digit
 *            of k also appears as a base‑6 digit of k
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Return true if k satisfies the property. */
static bool valid_k(seq_t k)
{
    /* Check whether k is of the form 2^m−1 (binary all 1’s). */
    bool all_ones = ((k & (k + 1)) == 0);

    /* Build a mask of digits present in the base‑6 representation. */
    unsigned mask = 0;
    seq_t t = k;
    while (t) {
        int d = (int)(t % 6);
        mask |= 1u << d;
        t /= 6;
    }

    /* The digit 1 must appear. */
    if (!(mask & (1u << 1)))
        return false;

    /* If the binary representation contains a 0, the base‑6
       representation must also contain a 0. */
    if (!all_ones && !(mask & 1u))
        return false;

    return true;
}

/* Return a(n). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 0;
    while (count < n) {
        ++k;
        if (valid_k(k))
            ++count;
    }
    return k;
}
