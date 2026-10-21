/*
 * A071939  a(n) = Max { sigma(k) : k = 1, 2, 3, ..., n } - Max { Phi(k) : k =
 *          1, 2, 3, ..., n }.
 * https://oeis.org/A071939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max{sigma(k)} - max{phi(k)} for 1 ≤ k ≤ n
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation computes sigma and phi for every integer
   1…n on the fly and keeps the running maxima.  It does not
   store any large arrays of seq_t values, which makes it
   valid for both C (with builtin integer types) and C++ (with a
   big-integer typename) without resorting to malloc on seq_t. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- Simple sieve to generate primes up to 20000 ----------
   (enough for factoring numbers up to a few million, much larger
   than the limits required for this sequence).  The primes are
   stored in a static array that is filled only once.  */
static uint32_t primes[20000];
static size_t primes_cnt = 0;

static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    const int limit = 20000;
    bool *is_prime = (bool *)malloc(limit + 1);
    if (!is_prime) exit(EXIT_FAILURE);
    memset(is_prime, true, limit + 1);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= limit; j += i)
                is_prime[j] = false;
    for (int i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[primes_cnt++] = (uint32_t)i;
    free(is_prime);
    done = true;
}

/* ---------- Compute sigma and phi of a single integer ----------
   The result are stored in the pointed-to seq_t variables.  */
static void sigma_phi(idx_t n, seq_t *sigma, seq_t *phi)
{
    idx_t x = n;
    seq_t s_val = 1;
    seq_t p_val = n;           /* phi is initialised to the number itself */

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= (uint64_t)x; ++i) {
        uint32_t p = primes[i];
        if (x % p != 0) continue;
        /* count exponent of p in n */
        seq_t p_pow = 1;
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            p_pow *= (seq_t)p;
            ++exp;
        }
        /* sigma component: (p^{exp+1}-1)/(p-1) */
        seq_t sigma_factor = (p_pow * (seq_t)p - 1) / (p - 1);
        s_val *= sigma_factor;
        /* phi component: phi = phi / p * (p-1) */
        p_val = p_val / (seq_t)p * (seq_t)(p - 1);
    }
    if (x > 1) {              /* remaining prime factor */
        uint32_t p = (uint32_t)x;
        seq_t p_pow = (seq_t)p;
        seq_t sigma_factor = (p_pow * (seq_t)p - 1) / (p - 1);  /* = 1 + p */
        s_val *= sigma_factor;
        p_val = p_val / (seq_t)p * (seq_t)(p - 1);
    }
    *sigma = s_val;
    *phi   = p_val;
}

/* ---------- The sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, but safe */
    init_primes();

    seq_t max_sigma = 0;
    seq_t max_phi   = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t s, p;
        sigma_phi(k, &s, &p);
        if (s > max_sigma) max_sigma = s;
        if (p > max_phi)   max_phi   = p;
    }
    return max_sigma - max_phi;
}
