/*
 * A141563  Primes of the form 2*3*5*7*n+79.
 * https://oeis.org/A141563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form 210*n + 79
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   grow_seq, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t num_found = 0;
    static idx_t curr_n = 0; /* candidate n in 210*n + 79 */

    if (n == 0)
        return 0;

    while (num_found < n) {
        seq_t cand = 210 * ((seq_t)curr_n) + 79;
        if (isPrimeGeneric(cand)) {
            if (num_found >= cache_cap)
                grow_seq(&cache, &cache_cap, num_found + 1);
            cache[num_found] = cand;
            ++num_found;
        }
        ++curr_n;
    }
    return cache[n - 1];
}
