/*
 * A074198  Largest k such that 1!*2!*3!*...*k! divides n!.
 * https://oeis.org/A074198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the largest k such that (1! * 2! * … * k!) divides n!
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation pre‑computes the prime exponents of each factorial
 * up to a fixed maximum and the cumulative exponents of the product
 * of factorials.  For a given n a binary search on k uses these
 * pre‑computed tables. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */

static const uint32_t NMAX = 20000;  /* must be >= any n used by the tests */

static bool tables_ready = false;
static int  prime_cnt   = 0;
static int  *prime_list = NULL;           /* primes up to NMAX  */
static unsigned short *fact_exp = NULL;   /* [ (n*prime_cnt) + pi ] */
static unsigned short *prod_exp = NULL;   /* cumulative product exponents */

/* ------------------------------------------------------------------ */

static void init_tables(void)
{
    if (tables_ready) return;

    /* ---- sieve primes up to NMAX ---------------------------------- */
    bool *is_prime = (bool*)calloc(NMAX + 1, sizeof(bool));
    if (!is_prime) return;
    for (uint32_t i = 2; i <= NMAX; ++i) is_prime[i] = true;
    for (uint32_t i = 2; i * i <= NMAX; ++i) {
        if (!is_prime[i]) continue;
        for (uint32_t j = i * i; j <= NMAX; j += i) is_prime[j] = false;
    }
    /* count primes */
    for (uint32_t i = 2; i <= NMAX; ++i) if (is_prime[i]) ++prime_cnt;
    prime_list = (int*)malloc(prime_cnt * sizeof(int));
    if (!prime_list) { free(is_prime); return; }
    int idx = 0;
    for (uint32_t i = 2; i <= NMAX; ++i) if (is_prime[i]) prime_list[idx++] = (int)i;
    free(is_prime);

    /* ---- allocate exponent tables --------------------------------- */
    fact_exp = (unsigned short*)malloc((NMAX + 1) * prime_cnt * sizeof(unsigned short));
    prod_exp = (unsigned short*)malloc((NMAX + 1) * prime_cnt * sizeof(unsigned short));
    if (!fact_exp || !prod_exp) {
        free(prime_list);
        fact_exp = prod_exp = NULL;
        return;
    }
    /* fill tables */
    unsigned short *cur_fact = (unsigned short*)calloc(prime_cnt, sizeof(unsigned short));
    unsigned short *cur_prod = (unsigned short*)calloc(prime_cnt, sizeof(unsigned short));
    if (!cur_fact || !cur_prod) return;

    for (uint32_t n = 1; n <= NMAX; ++n) {
        uint32_t x = n;
        for (int pi = 0; pi < prime_cnt; ++pi) {
            int p = prime_list[pi];
            if ((uint32_t)p > x) break;
            unsigned short e = 0;
            while (x % p == 0) { ++e; x /= p; }
            cur_fact[pi] += e;
        }
        for (int pi = 0; pi < prime_cnt; ++pi) {
            fact_exp[n * prime_cnt + pi] = cur_fact[pi];
            cur_prod[pi] += fact_exp[n * prime_cnt + pi];
            prod_exp[n * prime_cnt + pi] = cur_prod[pi];
        }
    }
    free(cur_fact);
    free(cur_prod);

    tables_ready = true;
}

/* ------------------------------------------------------------------ */

static bool test_divisible(uint32_t n, uint32_t k)
{
    if (k > NMAX || n > NMAX) return false;
    for (int pi = 0; pi < prime_cnt; ++pi) {
        unsigned short e_n   = fact_exp[n * prime_cnt + pi];
        unsigned short e_prod = prod_exp[k * prime_cnt + pi];
        if (e_n < e_prod) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    init_tables();                     /* lazily build tables on first use */
    if (!tables_ready) return (seq_t)0; /* failure path – should not happen */

    /* if n is beyond the pre‑computed range, we fall back to a simple
     * calculation that uses Legendre's formula directly.  This keeps
     * the code correct even for large n, though it is only used if the
     * tests exceed NMAX. */
    if (n > NMAX) {
        /* binary search with on‑the‑fly exponent calculation */
        uint32_t low = 1, high = (uint32_t)n, ans = 1;
        while (low <= high) {
            uint32_t mid = low + (high - low) / 2;
            bool ok = true;
            for (int pi = 0; pi < prime_cnt; ++pi) {
                int p = prime_list[pi];
                /* exponent of p in n! */
                uint32_t exp_n = 0, t = (uint32_t)n;
                while (t) { t /= p; exp_n += t; }
                /* exponent of p in product 1!*…*mid! */
                uint32_t exp_prod = 0;
                /* use Legendre's formula on each factorial and sum */
                for (uint32_t i = 1; i <= mid; ++i) {
                    uint32_t tt = i;
                    while (tt) { tt /= p; exp_prod += tt; }
                }
                if (exp_n < exp_prod) { ok = false; break; }
            }
            if (ok) { ans = mid; low = mid + 1; }
            else     high = mid - 1;
        }
        return (seq_t)ans;
    }

    /* binary search over pre‑computed tables */
    uint32_t low = 1, high = (uint32_t)n, ans = 1;
    while (low <= high) {
        uint32_t mid = low + (high - low) / 2;
        if (test_divisible((uint32_t)n, mid)) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (seq_t)ans;
}
