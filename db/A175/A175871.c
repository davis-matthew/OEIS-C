/*
 * A175871  a(0) = 2; a(n) = a(n - 1) * 3 + 1 if a(n - 1) is prime, or a(n -
 *          1) / (smallest prime factor) if it is composite.
 * https://oeis.org/A175871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Recurrence: a(0)=2; a(n)= a(n-1)*3+1 if a(n-1) is prime,
   else a(n-1)/(smallest prime factor of a(n-1)).  The sequence
   is periodic after 16 iterations. */

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

static uint64_t smallest_prime_factor_uint64(uint64_t n)
{
    if (n % 2 == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return d;
    return n; /* n is prime, but this function is only called when n is composite */
}

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t* cache = NULL;
    static idx_t computed = 0;   /* number of terms stored (next to compute) */
    static size_t capacity = 0;  /* allocated capacity */

    /* initialise cache with a(0) if not already done */
    if (cache == NULL)
    {
        capacity = 16;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure, unlikely */
        cache[0] = (seq_t)2;
        computed = 1;
    }

    /* grow cache if necessary */
    if (n >= computed)
    {
        size_t required = (size_t)(n + 1);
        if (required > capacity)
        {
            size_t newcap = capacity ? capacity : 16;
            while (newcap < required)
                newcap <<= 1;
            seq_t* tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* allocation failure */
            cache = tmp;
            capacity = newcap;
        }

        /* compute terms from computed up to n */
        for (idx_t i = computed; i <= n; ++i)
        {
            seq_t prev = cache[i - 1];
            uint64_t prev_u = (uint64_t)prev;       /* value fits in 64 bits */
            seq_t next;
            if (is_prime_uint64(prev_u))
            {
                next = prev * (seq_t)3 + (seq_t)1;
            }
            else
            {
                uint64_t spf = smallest_prime_factor_uint64(prev_u);
                seq_t spf_seq = (seq_t)spf;
                next = prev / spf_seq;
            }
            cache[i] = next;
        }
        computed = (idx_t)(n + 1);
    }

    return cache[n];
}
