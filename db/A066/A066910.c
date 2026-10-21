/*
 * A066910  a(1) = 1; a(n+1) = (sum{k=1 to n} a(k) ) (mod n).
 * https://oeis.org/A066910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1;
 * Range:     u8 n <= 31, u16 n <= 501, u32 n <= 501, u64 n <= 501,
 *            u128 n <= 501, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n+1) = (sum_{k=1}^n a(k)) mod n  */

/* Computes the n-th term of sequence A066910.
   Uses a static cache so repeated calls are efficient. */
#include <stdlib.h>
extern unsigned long long _mul_mod64(unsigned long long a, unsigned long long b,
                                   unsigned long long mod);

seq_t a(idx_t n)
{
    /* static cache: index 1..cache_size */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* last computed index */
    static seq_t sum_of_a = 0;     /* sum of terms 1..cache_size */

    if (n < 1)
        return 0;                  /* undefined for n==0 */

    /* initialise cache on first use */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);   /* index 0 unused */
        if (!cache) return 0;                         /* allocation failed */
        cache[1] = 1;                                 /* a(1) */
        sum_of_a = 1;
    }

    /* extend cache if needed */
    if (n > cache_size) {
        idx_t i;
        for (i = cache_size + 1; i <= n; ++i) {
            seq_t next = sum_of_a % (seq_t)(i - 1);   /* a(i) */
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * (i + 1));
            if (!cache) return 0;                    /* allocation failed */
            cache[i] = next;
            sum_of_a += next;
        }
        cache_size = n;
    }
    return cache[n];
}
