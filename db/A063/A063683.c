/*
 * A063683  Integers formed from the reduced residue sets of even numbers and
 *          Fibonacci numbers.
 * https://oeis.org/A063683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A063683 – a(n) = Σ_{i | gcd(i,2n)=1} Fib(i)  (Fib(1)=1, Fib(2)=1) */

#include <stdint.h>
#include <stdlib.h>

/* own 64‑bit gcd – no external header required */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* cached Fibonacci values: fib_cache[k] holds Fib(k) (k ≥ 1) */
static seq_t *fib_cache = NULL;   /* array pointer */
static idx_t  fib_cache_len = 0;   /* largest index currently stored */

/* ensure fib_cache contains Fib(k) for 1 ≤ k ≤ need */
static void ensure_fib(idx_t need)
{
    if (need <= fib_cache_len)
        return;

    /* grow the array to accommodate indices up to `need` */
    size_t new_size = (size_t)need + 1; /* indices 0..need (index 0 unused) */
    if (fib_cache == NULL) {
        fib_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!fib_cache) return;   /* allocation failure – leave cache empty */
        /* initialise first two Fibonacci numbers */
        fib_cache[0] = 0;            /* unused */
        fib_cache[1] = 1;            /* Fib(1) */
        fib_cache[2] = 1;            /* Fib(2) */
        fib_cache_len = 2;
    } else {
        fib_cache = (seq_t *)realloc(fib_cache, new_size * sizeof(seq_t));
        if (!fib_cache) return; /* allocation failure – leave cache as is */
    }

    /* compute missing Fibonacci values */
    for (idx_t i = fib_cache_len + 1; i <= need; ++i) {
        /* fib_cache[0] is unused, so i-1 ≥ 1 and i-2 ≥ 0 */
        fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
    }
    fib_cache_len = need;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t two_n = n * 2ull;          /* 2n – safe in 64 bits for given limits */

    /* build Fibonacci table as needed */
    ensure_fib(two_n);

    seq_t sum = 0;
    uint64_t two_val = (uint64_t)two_n;

    /* iterate over i = 1 .. 2n */
    for (idx_t i = 1; i <= two_n; ++i) {
        if (gcd_uint64((uint64_t)i, two_val) == 1) {
            sum += fib_cache[(size_t)i];
        }
    }

    return sum;
}
