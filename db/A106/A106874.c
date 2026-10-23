/*
 * A106874  Primes of the form x^2+xy+8y^2, with x and y nonnegative.
 * https://oeis.org/A106874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime of the form x^2 + x*y + 8*y^2 with x, y ≥ 0.
 *            The function enumerates all numbers of that form up to a
 *            dynamically increasing limit, keeps the primes in sorted order,
 *            and returns the n‑th element. No external sequencing data is
 *            used
 * Range:     u8 none, u16 none, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------- Prime test (deterministic Miller‑Rabin) ------------- */

static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    while (e) {
        if (e & 1)
            res = mod_mul(res, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
         31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL,
         450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------- Global storage for computed primes ------------- */

static seq_t *prime_vals = NULL;   /* sorted list of primes */
static size_t prime_cap = 0;       /* allocated capacity */
static size_t prime_count = 0;     /* number of stored primes */
static uint64_t current_limit = 0; /* last limit used to generate primes */

/* comparison function for qsort (works for any unsigned seq_t) */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* generate all primes of the form x^2 + x*y + 8*y^2 ≤ limit,
   store them sorted in prime_vals.  If the array was allocated before,
   it is resized as necessary.  */
static void generate_primes_up_to(uint64_t limit)
{
    /* temporary dynamic array for the primes found in this pass */
    size_t tmp_cap = 1024;
    size_t tmp_sz = 0;
    seq_t *tmp = (seq_t *)malloc(tmp_cap * sizeof(seq_t));
    if (!tmp) return; /* allocation failure – should never happen */

    for (uint64_t y = 0;; ++y) {
        uint64_t y2 = 8ULL * y * y;
        if (y2 > limit) break;          /* no further y gives values ≤ limit */
        for (uint64_t x = 0;; ++x) {
            uint64_t val = x * x + x * y + y2;
            if (val > limit) break;      /* values increase with x      */
            if (val >= 2 && is_prime_uint64(val)) {
                if (tmp_sz == tmp_cap) {
                    tmp_cap *= 2;
                    seq_t *tmp2 = (seq_t *)realloc(tmp, tmp_cap * sizeof(seq_t));
                    if (!tmp2) { /* allocation failed */
                        free(tmp);
                        return;
                    }
                    tmp = tmp2;
                }
                tmp[tmp_sz++] = (seq_t)val;
            }
        }
    }

    /* sort the temporary array and remove duplicates */
    qsort(tmp, tmp_sz, sizeof(seq_t), cmp_seq);
    size_t uniq = 0;
    for (size_t i = 0; i < tmp_sz; ++i) {
        if (uniq == 0 || tmp[i] != tmp[uniq - 1])
            tmp[uniq++] = tmp[i];
    }
    tmp_sz = uniq;

    /* resize the global array if necessary */
    if (prime_cap < tmp_sz) {
        seq_t *old = prime_vals;
        seq_t *newarr = (seq_t *)realloc(old, tmp_sz * sizeof(seq_t));
        if (!newarr) { /* allocation failure */
            free(tmp);
            return;
        }
        prime_vals = newarr;
        prime_cap = tmp_sz;
    }

    /* copy the sorted unique primes into the global array */
    for (size_t i = 0; i < tmp_sz; ++i)
        prime_vals[i] = tmp[i];
    prime_count = tmp_sz;
    current_limit = limit;

    free(tmp);
}

/* ensure that prime_vals contains at least n primes.  If not, increase
   the limit and regenerate.  */
static void ensure_n(idx_t n)
{
    if (n == 0) return;
    if (!prime_vals || prime_count < (size_t)n) {
        uint64_t limit = current_limit ? current_limit : 1000000ULL;
        while (prime_count < (size_t)n) {
            generate_primes_up_to(limit);
            if (prime_count >= (size_t)n) break;
            limit *= 2;
        }
    }
}

/* ------------- Main sequence function ------------- */

seq_t a(idx_t n)
{
    /* n is 1‑based */
    ensure_n(n);
    return prime_vals[n - 1];
}
