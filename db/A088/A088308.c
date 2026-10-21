/*
 * A088308  2 followed by list of composite numbers mod 10.
 * https://oeis.org/A088308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1)=2, and for n≥2, a(n) = (c_{n-1} mod 10)
 * where c_k is the k‑th composite number (4,6,8,9,10,…).
 */

/*  Required headers  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*  Helper: simple primality test (works for numbers up to a few hundred thousands) */
static inline bool is_prime_simple(idx_t v) {
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    for (idx_t i = 3; i * i <= v; i += 2) {
        if (v % i == 0) return false;
    }
    return true;
}

/*  Static cache for computed last digits of composites  */
static seq_t   *composite_last_digit_cache = NULL;
static size_t  cache_capacity           = 0;
static idx_t   composites_found         = 0;   /* number of composites stored */
static idx_t   next_to_check           = 4;    /* integer to test next */

/*  Ensure that the cache can hold at least 'need' entries  */
static void ensure_cache_capacity(size_t need) {
    if (cache_capacity >= need) return;
    /*  Grow: double until enough  */
    while (cache_capacity < need) {
        cache_capacity = (cache_capacity == 0) ? 1024 : (cache_capacity << 1);
    }
    composite_last_digit_cache = (seq_t *)realloc(
        composite_last_digit_cache, cache_capacity * sizeof(seq_t));
    /*  In the unlikely event of allocation failure we let the runtime crash –
        the problem specification guarantees that calls will be small enough.  */
}

/*  Generate composites until we have at least 'needed' composites stored  */
static void generate_composites(idx_t needed) {
    ensure_cache_capacity(needed);
    while (composites_found < needed) {
        /*  Skip primes and count composites  */
        if (!is_prime_simple(next_to_check)) {
            composite_last_digit_cache[composites_found] = (seq_t)(next_to_check % 10);
            ++composites_found;
        }
        ++next_to_check;
    }
}

/*  Main sequence function  */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for 0 but safer to return 0 */
    if (n == 1) return 2;            /* first term is the constant 2   */

    /*  For n≥2, we need the (n-1)th composite number's last digit  */
    idx_t needed = n - 1;            /* number of composites required */
    if (composites_found < needed) {
        generate_composites(needed);
    }
    return composite_last_digit_cache[n - 2];   /* 0‑based index in cache   */
}
