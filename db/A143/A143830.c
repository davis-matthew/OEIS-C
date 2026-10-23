/*
 * A143830  Primes of the form 12*n^2-1.
 * https://oeis.org/A143830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 33, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Returns the n‑th prime of the form 12*k^2 – 1 with a(1) = 11.
 * The sequence a(n) consists of all primes that can be written as
 * 12*k^2 – 1 for some integer k>0, enumerated in increasing k.
 */

#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1 – values for n<1 are undefined. */
    if (n == 0) return 0;

    idx_t found = 0;          /* how many primes we've found so far */
    idx_t k = 1;              /* current k to test */

    while (1) {
        /* compute 12*k^2-1 using a wider type to avoid overflow
           before we cast back to seq_t for primality testing.    */
        unsigned __int128 cand128 = (unsigned __int128)12 * k * k - 1;
        seq_t candidate = (seq_t)cand128;

        if (is_prime_big(candidate)) {
            ++found;
            if (found == n) {
                return candidate;   /* nth term */
            }
        }
        ++k;
    }
}
