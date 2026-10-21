/*
 * A060038  Triangle T(n,k) = 0 if k-th prime is a square mod the n-th prime,
 *          otherwise 1, for 1<=k<n.
 * https://oeis.org/A060038
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) is the n-th element (offset 2) of the flattened triangle *
 *            T defined by T(n,k)=0 if the k‑th prime is a quadratic residue
 *            modulo the * n‑th prime, otherwise 1; 1≤k<n, n≥2. The function
 *            generates the first * 20000 primes, builds each row on demand,
 *            records the 0/1 values in a * compact array,...
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* --- static data ----------------------------------------------------- */
static size_t seq_len  = 0;        /* number of terms already stored   */
static size_t seq_cap  = 0;        /* current capacity of seq_arr      */
static uint8_t *seq_arr = NULL;   /* array holding 0/1 values        */

static uint32_t primes[20000];     /* first 20000 primes              */
static size_t   primes_len = 0;    /* number of primes stored         */
static size_t   max_prepared = 1;  /* last row whose terms are stored */

/* --- helper to build prime table ----------------------------------- */
static void init_primes(void)
{
    if (primes_len > 0) return;   /* already built */

    const size_t LIMIT = 300000;   /* > p_20000 */
    uint8_t *sieve = (uint8_t*)calloc(LIMIT + 1, 1);
    if (!sieve) return;           /* allocation failed */

    /* mark all numbers as potential primes */
    for (size_t i = 2; i <= LIMIT; ++i) sieve[i] = 1;

    /* sieve of Eratosthenes */
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 0;
        }
    }

    /* collect primes */
    for (size_t i = 2; i <= LIMIT && primes_len < 20000; ++i) {
        if (sieve[i]) primes[primes_len++] = (uint32_t)i;
    }

    free(sieve);
}

/* --- helper to ensure capacity of seq_arr --------------------------- */
static void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? (seq_cap << 1) : 1024;
    while (new_cap < need) new_cap <<= 1;
    uint8_t *new_arr = (uint8_t*)realloc(seq_arr, new_cap);
    if (!new_arr) return;            /* keep old buffer on failure */
    seq_arr = new_arr;
    seq_cap = new_cap;
}

/* --- public API ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;      /* behaviour for offset < 2 undefined */

    /* initialise primes if needed */
    if (primes_len == 0) init_primes();

    /* build rows up to requested n */
    if (n > max_prepared) {
        for (size_t row = max_prepared + 1; row <= (size_t)n; ++row) {
            /* prime for this row: P_row */
            uint32_t p_n = primes[row - 1];

            /* pre‑compute quadratic residues modulo p_n */
            size_t resid_size = (size_t)p_n;
            uint8_t *resid = (uint8_t*)calloc(resid_size, 1);
            if (!resid) return (seq_t)0;   /* allocation error */

            uint32_t half = (p_n - 1) >> 1;   /* floor((p_n-1)/2) */
            for (uint32_t x = 1; x <= half; ++x) {
                uint32_t r = (uint32_t)(((uint64_t)x * x) % p_n);
                resid[r] = 1;
            }

            /* generate the row elements T(row,k), 1 ≤ k < row   */
            size_t needed = seq_len + (row - 1);
            ensure_seq_capacity(needed);

            for (size_t k = 1; k < row; ++k) {
                uint32_t pk = primes[k - 1];   /* k‑th prime (1‑based) */
                seq_arr[seq_len++] = resid[pk] ? 0 : 1;
            }
            free(resid);
        }
        max_prepared = n;
    }

    /* terms are stored starting with a(2) → seq_arr[0] */
    return (seq_t)seq_arr[(size_t)n - 2];
}
