/*
 * A180685  Primes consisting of all of the cube digits (i.e., 0, 1 and 8) at
 *          least once.
 * https://oeis.org/A180685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th prime that contains at least one of each decimal
 *            digit 0,1,8 and no other digits (search)
 * Range:     u8 none, u16 n <= 6, u32 n <= 1479, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */
#include "../utils/isPrimeGeneric.h"

static const unsigned digit_bit_map[10] = {
    1u,            /* 0 */
    2u,            /* 1 */
    0u,            /* 2 */
    0u,            /* 3 */
    0u,            /* 4 */
    0u,            /* 5 */
    0u,            /* 6 */
    0u,            /* 7 */
    4u,            /* 8 */
    0u             /* 9 */
};

static idx_t       target_n;   /* target index to reach */
static idx_t       found_n;    /* number of found primes so far */
static seq_t       nth_prime;  /* result when target reached */

/* Recursive generator: pos = current position, len = total digits. */
static bool gen_rec(int pos, int len, seq_t value, unsigned mask)
{
    if (found_n >= target_n)            /* stop once target reached */
        return true;

    if (pos == len) {                   /* reached full length */
        if (mask == 7u && isPrimeGeneric(value)) { /* 0,1,8 all present */
            ++found_n;
            if (found_n == target_n) {
                nth_prime = value;
                return true;          /* terminate all recursion */
            }
        }
        return false;
    }

    /* Determine possible digits for this position. */
    int digits[3];
    int nd;
    if (pos == len - 1) {               /* last digit must be 1 (odd prime) */
        digits[0] = 1;
        nd = 1;
    } else if (pos == 0) {              /* leading digit cannot be 0 */
        digits[0] = 1;
        digits[1] = 8;
        nd = 2;
    } else {                            /* middle digits */
        digits[0] = 0;
        digits[1] = 1;
        digits[2] = 8;
        nd = 3;
    }

    for (int i = 0; i < nd; ++i) {
        int d = digits[i];
        if (gen_rec(pos + 1, len,
                    value * 10 + (seq_t)d,
                    mask | digit_bit_map[d]))
            return true;               /* propagate stop signal */
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for offset 1, but safe */

    target_n = n;
    found_n  = 0;

    for (int len = 1; /* unlimited */ ; ++len) {
        if (gen_rec(0, len, 0, 0))
            break;                      /* target reached, nth_prime set */
    }
    return nth_prime;
}
