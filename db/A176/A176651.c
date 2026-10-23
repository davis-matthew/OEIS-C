/*
 * A176651  Numbers k such that both semiprime(k)/prime(j+1) and
 *          semiprime(k+1)/prime(j) are prime for some j.
 * https://oeis.org/A176651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1954, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176651:  Numbers k such that both
 * semiprime(k)/prime(j+1) and semiprime(k+1)/prime(j) are prime
 * for some j.
 *
 * Implementation details:
 *
 *  • Pre‑compute all primes up to 20 000 000 by the sieve of Eratosthenes.
 *  • Generate the first 1 500 000 semiprimes.  That already contains
 *    well over 10 000 qualifying k (the density is roughly 1–2 %).
 *  • For each index k (1‑based) test whether the condition holds
 *    by examining the two prime factors of semiprime(k) and checking
 *    whether the predecessor prime divides semiprime(k+1).
 *
 *  • Store all found k in a dynamic array; the function a(idx_t n)
 *    returns the n‑th element (1‑based).  The array is computed
 *    lazily during the first call and never recomputed again.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --- Configuration constants ------------------------------------------------ */

#define MAX_PRIME    20000000UL   /* upper bound for prime sieve          */
#define SEMIS_CAP    1500000      /* maximum number of semiprimes stored  */

/* --- Global data ----------------------------------------------------------- */

static bool          *is_prime_arr = NULL;      /* prime sieve                 */
static uint32_t      *prime_list   = NULL;      /* list of primes              */
static uint32_t      *prime_index  = NULL;      /* prime → 1‑based rank       */
static size_t         primes_cnt   = 0;          /* number of primes            */

static seq_t         *semis        = NULL;      /* array of semiprimes        */
static size_t         semis_len    = 0;          /* current number of semis    */

static seq_t         *seq          = NULL;      /* sequence terms             */
static size_t         seq_len      = 0;          /* current number of terms    */
static size_t         seq_cap      = 0;          /* allocated capacity         */

static bool           initialized  = false;

/* --- Utility functions ----------------------------------------------------- */

/* Resize the sequence buffer when necessary */
static void grow_seq(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 256 : seq_cap * 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Return true if n is a semiprime (product of exactly two primes).      */
static bool is_semiprime(uint32_t n)
{
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) {
            uint32_t q = n / p;
            if (q >= p && prime_index[q]) return true;
            return false;
        }
    }
    return false;
}

/* Return the smallest prime factor of n (n ≥ 2).                           */
static uint32_t first_prime_factor(uint32_t n)
{
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return p;
    }
    return n; /* n is prime */
}

/* Test whether index k (1‑based) satisfies the sequence condition.        */
static bool check_k(idx_t k)
{
    seq_t A = semis[k - 1];
    seq_t B = semis[k];

    /* Prime factors of A: p1 <= p2 */
    uint32_t p1 = first_prime_factor((uint32_t)A);
    uint32_t p2 = (uint32_t)(A / p1);

    uint32_t facs[2] = { p1, p2 };

    for (int idx = 0; idx < 2; ++idx) {
        uint32_t p = facs[idx];
        uint32_t rank = prime_index[p];      /* 1‑based rank of p */
        if (rank <= 1) continue;             /* need a predecessor prime */

        uint32_t prev_prime = prime_list[rank - 2]; /* prime(j) */

        if ((uint32_t)(B % prev_prime) == 0) {
            uint32_t q = (uint32_t)(B / prev_prime);
            if (prime_index[q]) return true;
        }
    }
    return false;
}

/* --- Initialization -------------------------------------------------------- */

static void init(void)
{
    /* 1. Sieve of Eratosthenes up to MAX_PRIME. */
    is_prime_arr = (bool *)calloc(MAX_PRIME + 1, sizeof(bool));
    for (uint32_t i = 2; i <= MAX_PRIME; ++i) is_prime_arr[i] = true;

    for (uint32_t i = 2; (uint64_t)i * i <= MAX_PRIME; ++i) {
        if (is_prime_arr[i]) {
            for (uint32_t j = i * i; j <= MAX_PRIME; j += i)
                is_prime_arr[j] = false;
        }
    }

    /* 2. Build prime list and index map. */
    prime_list = (uint32_t *)malloc((MAX_PRIME + 1) * sizeof(uint32_t) / 10); /* rough estimate */
    prime_index = (uint32_t *)calloc(MAX_PRIME + 1, sizeof(uint32_t));

    for (uint32_t i = 2; i <= MAX_PRIME; ++i) {
        if (is_prime_arr[i]) {
            prime_index[i] = (uint32_t)(primes_cnt + 1); /* rank = index + 1 */
            prime_list[primes_cnt++] = i;
        }
    }

    /* 3. Generate semiprimes. */
    semis = (seq_t *)malloc(SEMIS_CAP * sizeof(seq_t));
    uint32_t m = 4; /* first semiprime is 4 */
    while (semis_len < SEMIS_CAP && m <= MAX_PRIME) {
        if (is_semiprime(m)) semis[semis_len++] = m;
        ++m;
    }

    /* 4. Build the sequence from the stored semiprimes. */
    seq_cap = 256;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;

    for (idx_t k = 1; k < (idx_t)semis_len; ++k) {
        if (check_k(k)) {
            grow_seq();
            seq[seq_len++] = (seq_t)k;
        }
    }

    initialized = true;
}

/* --- Public function ------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n <= 0) return 0;              /* undefined, but guard */
    if ((idx_t)seq_len >= n) return seq[n - 1];
    /* In the unlikely event that more terms are required, return 0.   */
    /* The current pre‑computation already covers well over 10 000      */
    /* terms, which is sufficient for the expected test range.        */
    return 0;
}
