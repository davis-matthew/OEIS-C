/*
 * A053006  Numbers m for which there exist d(1),...,d(m), each in {0,1}, such
 *          that Sum_{i=1..m-k} d(i)*d(i+k) is odd for all k=0,...,m-1.
 * https://oeis.org/A053006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A036259(n)+1)/2, implemented by generating all integers
 *            m such that the multiplicative order of 2 mod (2m‑1) is odd
 * Range:     u8 n <= 37, u16 n <= 5601, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* modular exponentiation : base^exp mod mod, with 128‑bit  */
/* intermediate for safety.                                 */
static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base  = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* ------------------------------------------------------------ */
/* Prime sieve (up to LIMIT) used for all factorizations.       */
static int   *prime_list   = NULL;   /* array of primes          */
static size_t prime_count  = 0;       /* number of primes found   */
static size_t prime_cap    = 0;       /* allocated capacity       */
static bool   primes_inited = false; /* flag                       */

static void init_primes(void)
{
    if (primes_inited)
        return;

    /* LIMIT chosen generously large enough to factor numbers
       up to about 2*10^6 (far beyond the required range).   */
    const size_t LIMIT = 200000;
    bool *sieve = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!sieve)
        return; /* allocation failure – unlikely in contest env */

    for (size_t i = 2; i <= LIMIT; ++i)
        sieve[i] = true;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = false;
        }
    }

    /* Initially allocate some space for primes.          */
    prime_cap = 256;
    prime_list = (int*)malloc(prime_cap * sizeof(int));
    if (!prime_list) {
        free(sieve);
        return;
    }

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (sieve[i]) {
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                int *tmp = (int*)realloc(prime_list, prime_cap * sizeof(int));
                if (!tmp) {
                    free(prime_list);
                    prime_list = NULL;
                    prime_count = 0;
                    free(sieve);
                    return;
                }
                prime_list = tmp;
            }
            prime_list[prime_count++] = (int)i;
        }
    }
    free(sieve);
    primes_inited = true;
}

/* ------------------------------------------------------------ */
/* phi(n) using the pre‑generated prime list.  */
static uint64_t phi_of(uint64_t n)
{
    uint64_t result = n;
    uint64_t temp   = n;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        if (temp % p == 0) {
            while (temp % p == 0)
                temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1)
        result -= result / temp;
    return result;
}

/* ------------------------------------------------------------ */
/* Determine whether the multiplicative order of 2 modulo mod
   is odd.  mod is guaranteed to be odd.                       */
static bool order_of_2_is_odd(uint64_t mod)
{
    if (mod == 1ULL)      /* order = 1, odd */
        return true;

    uint64_t phi = phi_of(mod);

    /* obtain the set of distinct prime factors of phi  */
    uint64_t phi_temp = phi;
    uint64_t factors[32];
    size_t   fcnt = 0;

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= phi_temp; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        if (phi_temp % p == 0) {
            while (phi_temp % p == 0)
                phi_temp /= p;
            if (fcnt < sizeof(factors)/sizeof(factors[0]))
                factors[fcnt++] = p;
        }
    }
    if (phi_temp > 1 && fcnt < sizeof(factors)/sizeof(factors[0]))
        factors[fcnt++] = phi_temp;

    /* compute the multiplicative order of 2   */
    uint64_t order = phi;
    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t p = factors[i];
        while (order % p == 0) {
            if (mod_pow(2ULL, order / p, mod) == 1ULL)
                order /= p;
            else
                break;
        }
    }
    return (order & 1ULL) == 1ULL;
}

/* ------------------------------------------------------------ */
/* Cache for the generated sequence terms.                       */
static seq_t *seq_terms      = NULL;
static size_t seq_terms_len  = 0;  /* number of terms stored   */
static size_t seq_terms_cap  = 0;  /* allocated capacity       */
static uint64_t tested_m     = 0;  /* last m that was examined */

/* ------------------------------------------------------------ */
/* Main sequence function                                         */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n==0, safe for tests */

    init_primes();          /* ensure primes are ready */

    /* fill cache until we have at least n terms   */
    while (seq_terms_len < (size_t)n) {
        ++tested_m;
        uint64_t m   = tested_m;
        uint64_t mod = 2ULL * m - 1ULL;

        if (order_of_2_is_odd(mod)) {
            /* append m to the cache */
            if (seq_terms_len == seq_terms_cap) {
                size_t new_cap = seq_terms_cap ? seq_terms_cap << 1 : 16;
                seq_t *tmp = (seq_t*)realloc(seq_terms, new_cap * sizeof(seq_t));
                if (!tmp)       /* allocation failure – abort gracefully */
                    return 0;
                seq_terms = tmp;
                seq_terms_cap = new_cap;
            }
            seq_terms[seq_terms_len++] = (seq_t)m;
        }
    }
    return seq_terms[n - 1];
}
