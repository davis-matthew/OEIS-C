/*
 * A159951  Fibonacci integral quotients associated with the dividends in
 *          A159950 and the divisors in A003481.
 * https://oeis.org/A159951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Product_{k=1..4*n+2} Fibonacci(k))/(Sum_{k=1..4*n+2}
 *            Fibonacci(k))
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types supplied by the build system. */
extern seq_t a(idx_t n);
seq_t a(idx_t n) {
    /*  Cache for already computed Fibonacci numbers.  */
    static seq_t *fib = NULL;
    static size_t fib_cap = 0;
    static idx_t fib_max = 0;  /* Highest Fibonacci index that has been computed.  */

    /*  Cache for results a(1) .. a(max_cached).  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_cached = 0;

    /*  Current product of Fibonacci numbers 1 .. (4*max_cached + 2).  */
    static seq_t prod = 0;
    static idx_t prod_index = 0;   /* Highest Fibonacci index included in `prod`.  */

    /* Lazy initialisation of tables and product.  */
    if (fib == NULL) {
        fib_cap = 32;  /* initial capacity, will grow automatically.  */
        fib = (seq_t*)malloc(sizeof(seq_t)*(fib_cap+1));
        fib[0] = (seq_t)0;   /* unused */
        fib[1] = (seq_t)1;
        fib[2] = (seq_t)1;
        fib_max = 2;
    }

    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t*)malloc(sizeof(seq_t)*(cache_cap+1));
    }

    /* Ensure capacity for the required indices.  */
    if (n > cache_cap) {
        size_t new_cap = n + 32;
        cache = (seq_t*)realloc(cache, sizeof(seq_t)*(new_cap+1));
        cache_cap = new_cap;
    }

    /* Initialise the running product the first time.  */
    if (max_cached == 0) {
        prod = (seq_t)1;          /* product of zero terms  */
        prod_index = 0;           /* no Fibonacci indices multiplied yet  */
    }

    /*  Compute values up to requested `n`.  */
    for (idx_t cur = max_cached + 1; cur <= n; ++cur) {
        /* New upper bound for the product is 4*cur + 2.  */
        idx_t new_bound = 4*cur + 2;

        /* Make sure we can address Fibonacci numbers up to `new_bound`.  */
        if (new_bound > fib_max) {
            size_t need = new_bound + 1;
            while (need > fib_cap) fib_cap <<= 1;
            fib = (seq_t*)realloc(fib, sizeof(seq_t)*(fib_cap+1));
            for (idx_t i = fib_max + 1; i <= new_bound; ++i)
                fib[i] = fib[i-1] + fib[i-2];
            fib_max = new_bound;
        }

        /* Multiply all Fibonacci numbers that have not yet been incorporated
           into the product.  */
        for (idx_t i = prod_index + 1; i <= new_bound; ++i)
            prod *= fib[i];
        prod_index = new_bound;

        /* Compute the sum that appears in the denominator:  fib[4*cur+4] - 1.  */
        idx_t sum_idx = 4*cur + 4;
        if (sum_idx > fib_max) {
            size_t need = sum_idx + 1;
            while (need > fib_cap) fib_cap <<= 1;
            fib = (seq_t*)realloc(fib, sizeof(seq_t)*(fib_cap+1));
            for (idx_t i = fib_max + 1; i <= sum_idx; ++i)
                fib[i] = fib[i-1] + fib[i-2];
            fib_max = sum_idx;
        }
        seq_t sum_val = fib[sum_idx] - (seq_t)1;

        /* Store the result for this index.  */
        cache[cur] = prod / sum_val;
    }

    max_cached = n;
    return cache[n];
}
