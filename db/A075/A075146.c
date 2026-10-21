/*
 * A075146  n-th perfect power - n-th prime: pp(n) - prime(n).
 * https://oeis.org/A075146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = A001597(n) - A000040(n)
 * Range:     i8 n <= 1, i16 n <= 55, i32 n <= 1, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;
static seq_t *pp_arr = NULL;
static size_t pp_size = 0;
static size_t pp_cap = 0;
static uint64_t *primes = NULL;
static size_t primes_count = 0;

/* comparison function for std::qsort (C) or std::sort (C++) */
static int cmp_seq(const void *a, const void *b) {
    const seq_t *sa = (const seq_t *)a;
    const seq_t *sb = (const seq_t *)b;
    if (*sa < *sb) return -1;
    if (*sa > *sb) return 1;
    return 0;
}

/* append a perfect power to the dynamic array */
static void push_pp(seq_t v) {
    if (pp_size == pp_cap) {
        pp_cap = pp_cap ? pp_cap * 2 : 16;
        pp_arr = (seq_t *)realloc(pp_arr, pp_cap * sizeof(seq_t));
    }
    pp_arr[pp_size++] = v;
}

/* build prime list and perfect‑power list */
static void init(void) {
    if (initialized) return;

    /* ---- generate primes up to 200 000 (≈17 984 primes) ---- */
    const idx_t prime_limit = 200000;
    bool *is_prime = (bool *)calloc(prime_limit + 1, sizeof(bool));
    for (idx_t i = 2; i <= prime_limit; ++i) is_prime[i] = true;
    for (idx_t i = 2; (idx_t)i * i <= prime_limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= prime_limit; j += i) is_prime[j] = false;
        }
    }
    primes_count = 0;
    for (idx_t i = 2; i <= prime_limit; ++i) if (is_prime[i]) primes_count++;
    primes = (uint64_t *)malloc(primes_count * sizeof(uint64_t));
    for (idx_t i = 2, k = 0; i <= prime_limit; ++i) if (is_prime[i]) primes[k++] = (uint64_t)i;
    free(is_prime);

    /* ---- generate perfect powers up to 10¹² ---- */
    const seq_t limit = (seq_t)1000000000000ULL;     /* 1 000 000 000 000 */
    pp_cap = 1 << 10;
    pp_arr = (seq_t *)malloc(pp_cap * sizeof(seq_t));
    pp_size = 0;
    push_pp((seq_t)1);                               /* include 1 */

    for (idx_t base = 2; (seq_t)base * (seq_t)base <= limit; ++base) {
        seq_t pow = (seq_t)base * (seq_t)base;
        while (pow <= limit) {
            push_pp(pow);
            if (pow > limit / (seq_t)base) break;
            pow *= (seq_t)base;
        }
    }

    /* sort and remove duplicates */
    if (pp_size > 1) {
#if defined(__cplusplus)
        #include <algorithm>
        std::sort(pp_arr, pp_arr + pp_size);
#else
        qsort(pp_arr, pp_size, sizeof(seq_t), cmp_seq);
#endif
        size_t uniq = 0;
        for (size_t i = 0; i < pp_size; ++i) {
            if (uniq == 0 || pp_arr[i] != pp_arr[uniq - 1]) {
                pp_arr[uniq++] = pp_arr[i];
            }
        }
        pp_size = uniq;
    }

    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init();
    if (n == 0) return (seq_t)0;                 /* safe guard, although spec says n ≥ 1 */

    idx_t i = n - 1;
    if ((size_t)i >= pp_size || (size_t)i >= primes_count) return (seq_t)0;

    seq_t pp_val = pp_arr[i];
    seq_t prime_val = (seq_t)primes[i];
    return pp_val - prime_val;
}
