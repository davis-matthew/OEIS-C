/*
 * A097446  Concatenate consecutive prime-sided isosceles triangles.
 * https://oeis.org/A097446
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(p_{n-1}, p_n, p_n) for n ≥ 2, where p_k denotes
 *            the k‑th prime (p_1 = 2) and concat(a,b,c) means
 *            a·10^{2·digits(b)} + b·10^{digits(b)} + c
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ---------- prime cache ---------- */
typedef struct {
    uint32_t *data;   /* array of primes (1‑based indexing) */
    size_t    size;   /* number of primes stored */
    size_t    capacity;
} prime_cache_t;

static prime_cache_t primes = { NULL, 0, 0 };

/* helper: number of decimal digits of a 32‑bit unsigned integer */
static int digits_uint32(uint32_t x) {
    int d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

/* helper: 10^exp as seq_t */
static seq_t pow10_seq(uint32_t exp) {
    seq_t r = (seq_t)1;
    for (uint32_t i = 0; i < exp; ++i) r = r * (seq_t)10;
    return r;
}

/* generate primes up to 'limit' (inclusive) and store them in primes.cache */
static void generate_primes_up_to_limit(unsigned int limit) {
    uint8_t *sieve = (uint8_t*)malloc((size_t)limit + 1);
    if (!sieve) return;          /* allocation failure – abort silently */
    memset(sieve, 1, (size_t)limit + 1);
    if (limit >= 0) sieve[0] = 0;
    if (limit >= 1) sieve[1] = 0;

    unsigned int i, j;
    for (i = 2; (unsigned long long)i * i <= limit; ++i) {
        if (sieve[i]) {
            for (j = i * i; j <= limit; j += i) sieve[j] = 0;
        }
    }
    /* free previous cache and allocate new one */
    free(primes.data);
    primes.data = NULL;
    primes.size = 0;
    primes.capacity = 0;
    for (i = 2; i <= limit; ++i) if (sieve[i]) {
        if (primes.size == primes.capacity) {
            size_t newcap = primes.capacity ? primes.capacity * 2 : 16;
            uint32_t *tmp = (uint32_t*)realloc(primes.data, newcap * sizeof(uint32_t));
            if (!tmp) return; /* allocation failure */
            primes.data = tmp;
            primes.capacity = newcap;
        }
        primes.data[primes.size++] = i;
    }
    free(sieve);
}

/* ensure at least 'need' primes are in the cache (need ≥ 1) */
static void ensure_primes(size_t need) {
    if (need <= primes.size) return;
    unsigned int limit;
    if (need < 6) {
        limit = 15;
    } else {
        double n = (double)need;
        double estimate = n * (log(n) + log(log(n)));
        limit = (unsigned int)(estimate) + 3;
    }
    generate_primes_up_to_limit(limit);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    /* offset 2: a(2) is the first term */
    if (n < 2) return (seq_t)0;   /* behavior below offset is irrelevant */

    /* indices of the two consecutive primes (1‑based) */
    size_t idx1 = (size_t)(n - 2); /* prime p_{n-1} */
    size_t idx2 = (size_t)(n - 1); /* prime p_{n}  */

    ensure_primes(idx2 + 1); /* need at least idx2+1 primes (0‑based) */

    uint32_t p1 = primes.data[idx1];
    uint32_t p2 = primes.data[idx2];

    int d = digits_uint32(p2);          /* number of digits of p2 */
    seq_t pow10_d  = pow10_seq((uint32_t)d);
    seq_t pow10_d2 = pow10_d * pow10_d; /* 10^{2d} */

    seq_t result = (seq_t)p1 * pow10_d2
                 + (seq_t)p2 * pow10_d
                 + (seq_t)p2;

    return result;
}
