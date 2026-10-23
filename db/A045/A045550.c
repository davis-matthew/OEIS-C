/*
 * A045550  Numbers whose factorial has '8' as its final nonzero digit.
 * https://oeis.org/A045550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th positive integer m such that the last
 * non‑zero digit of the factorial m! is 8.
 *
 * The last non‑zero digit of n! in base 10 can be computed efficiently
 * using the recurrence 
 *   D(n) = D(floor(n/5)) * d(n mod 5) * 2^{floor(n/5)}  (mod 10),
 * where d(k) = {1,1,2,6,4} for k = 0,1,2,3,4 (the last non‑zero digit of k!).
 *
 * This algorithm runs in O(log₅ n) time per call and uses only
 * basic arithmetic, complying with the restrictions on the types.
 *
 * The function a(idx_t n) returns the n‑th member of the sequence A045550.
 */

#include <stdint.h>

/* Helper array: last non‑zero digit of k! for k = 0..4. */
static const seq_t last_nonzero_small[5] = {1, 1, 2, 6, 4};

/* Helper array: powers of 2 modulo 10 in the cycle 2^0,2^1,2^2,2^3. */
static const seq_t pow2_mod10[4] = {1, 2, 4, 8};

/* Compute the last non‑zero digit of n! (returns a value 1..9). */
static seq_t last_nonzero_digit(idx_t n)
{
    seq_t res = 1;
    while (n > 0) {
        idx_t rem = n % 5;
        res = (res * last_nonzero_small[rem]) % 10;
        idx_t e    = n / 5;          /* exponent for the factor 2. */
        seq_t pow2 = pow2_mod10[e % 4];
        res = (res * pow2) % 10;
        n /= 5;
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m     = 1;          /* candidate integer. */

    while (1) {
        if (last_nonzero_digit(m) == 8) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }
}
