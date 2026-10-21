/*
 * A069844  Number of distinct primes obtained by inserting a 3 at all
 *          possible places in n. This includes prefixing and suffixing.
 * https://oeis.org/A069844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct primes obtained by inserting a 3 * at
 *            any position in the decimal representation of n (including *
 *            prefixing and suffixing)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1; behaviour for n=0 is irrelevant */

    /* count decimal digits of n */
    idx_t tmp = n;
    int digits = 0;
    while (tmp != 0) {
        ++digits;
        tmp /= 10;
    }
    if (digits == 0) digits = 1;   /* n == 0 case, not used for offset 1 */

    /* precompute powers of 10 up to 10^(digits+1) */
    enum { MAX_D = 64 };   /* sufficient for any practical idx_t */
    idx_t pow10[MAX_D];
    pow10[0] = (idx_t)1;
    for (int i = 1; i <= digits + 1; ++i) {
        pow10[i] = pow10[i - 1] * (idx_t)10;
    }

    idx_t nseq = (idx_t)n;
    /* store primes found so far (there are at most digits+1 of them) */
    idx_t primes[64];
    int prime_cnt = 0;

    /* iterate over all possible insertion positions */
    for (int i = 0; i <= digits; ++i) {
        idx_t right = nseq % pow10[i];
        idx_t left  = nseq / pow10[i];

        idx_t inserted = left * pow10[i + 1] + (idx_t)3 * pow10[i] + right;

        if (isPrime(inserted)) {
            bool seen = false;
            for (int j = 0; j < prime_cnt; ++j) {
                if (primes[j] == inserted) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                primes[prime_cnt++] = inserted;
            }
        }
    }

    return (seq_t)prime_cnt;
}
