/*
 * A165887  Numbers n such that x^4 + y^4 != 0 (mod n) when both x, y
 *          (positive) are expressed mod n (i.e., 0 < x,y < n).
 * https://oeis.org/A165887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: odd squarefree integers with no prime factor ≡ 1 (mod 8). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime sieve data ----------------------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;
static bool    primes_ready = false;

/* Helper: build a small sieve of primes up to a fixed limit.  */
static void init_primes(void)
{
    if (primes_ready) return;
    /* Increase limit if you need more primes for larger n. */
    const size_t MAX_PRIME = 200000;          /* enough for n up to ~10⁵ */
    bool *is_composite = (bool *)calloc(MAX_PRIME + 1, sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);

    prime_cap = MAX_PRIME / 10 + 1;
    prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!prime_list) { free(is_composite); exit(EXIT_FAILURE); }

    for (size_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
                if (!tmp) { free(is_composite); free(prime_list); exit(EXIT_FAILURE); }
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = (uint64_t)i;
            if (i * i <= MAX_PRIME) {
                for (size_t j = i * i; j <= MAX_PRIME; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    primes_ready = true;
}

/* Helper: test whether m satisfies the sequence condition. */
static bool allowed(seq_t m)
{
    /* Must be odd. */
    if ((m & 1ULL) == 0) return false;

    seq_t x = m;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        seq_t p2 = (seq_t)p * (seq_t)p;
        if (p2 > x) break;                /* no further small factors */

        if (x % p == 0) {
            int exp = 0;
            while (x % p == 0) { x /= p; ++exp; }
            if (exp > 1) return false;     /* not squarefree */
            if ((p & 7ULL) == 1) return false;   /* p ≡ 1 (mod 8) */
        }
    }
    /* Remaining factor, if any. */
    if (x > 1) {
        if ((x % 8) == 1) return false;    /* prime ≡ 1 (mod 8) */
    }
    return true;
}

/* Storage for computed terms ------------------------------------------- */
static seq_t *terms = NULL;
static size_t terms_len   = 0;
static size_t terms_cap   = 0;

/* Ensure the array has at least 'cap' slots. */
static void ensure_terms_cap(size_t cap)
{
    if (terms_cap >= cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    while (new_cap < cap) new_cap *= 2;

#ifdef __cplusplus
    seq_t *new_arr = new seq_t[new_cap];
#else
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_arr) exit(EXIT_FAILURE);
    for (size_t i = 0; i < terms_len; ++i) new_arr[i] = terms[i];

#ifdef __cplusplus
    delete[] terms;
#else
    free(terms);
#endif
    terms      = new_arr;
    terms_cap  = new_cap;
}

/* Generate terms until we have at least n of them. */
static void generate(idx_t n)
{
    if (terms_len >= (size_t)n) return;
    if (!primes_ready) init_primes();

    ensure_terms_cap((size_t)n);

    static seq_t last_checked = 1;   /* previous odd candidate */
    while (terms_len < (size_t)n) {
        last_checked += 2;            /* next odd candidate */
        seq_t cand = last_checked;
        if (allowed(cand)) {
            terms[terms_len++] = cand;
        }
    }
}

/* Public API ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of bounds; safety */
    generate(n);
    return terms[n - 1];
}
