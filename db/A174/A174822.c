/*
 * A174822  Primes of form n^4 - n^2 - 1.
 * https://oeis.org/A174822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A174822 – Primes of the form n^4 – n^2 – 1  */
/*  The function returns the nth such prime (1‑based index).  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

#ifdef __cplusplus
# include <vector>
#endif

seq_t a(idx_t n) {
    /* Each sequence element is defined as:  p(k) = k^4 – k^2 – 1  for k ≥ 2
       where p(k) is prime.  This routine builds the list of these primes
       lazily and caches them for future calls.                                   */

#ifdef __cplusplus
    /* C++ implementation using std::vector – works when seq_t is a
       user‑defined big integer class.                                            */
    static std::vector<seq_t> cache;
    static idx_t next_k = 2;

    while (cache.size() < n) {
        seq_t k   = (seq_t)next_k;
        seq_t k2  = k * k;
        seq_t k4  = k2 * k2;
        seq_t val = k4 - k2 - (seq_t)1;
        if (isPrimeGeneric(val))
            cache.push_back(val);
        ++next_k;
    }
    return cache[n - 1];
#else
    /* C implementation – seq_t is a plain integer type, so realloc is safe. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t next_k = 2;
    static size_t len   = 0;

    while (len < n) {
        seq_t k   = (seq_t)next_k;
        seq_t k2  = k * k;
        seq_t k4  = k2 * k2;
        seq_t val = k4 - k2 - (seq_t)1;
        if (isPrimeGeneric(val)) {
            if (len == cap) {
                size_t newcap = cap ? cap * 2 : 8;
                seq_t *ptr    = realloc(cache, newcap * sizeof(seq_t));
                if (!ptr) return 0;   /* allocation failed */
                cache = ptr;
                cap   = newcap;
            }
            cache[len++] = val;
        }
        ++next_k;
    }
    return cache[n - 1];
#endif
}
