/*
 * A048682  a(n) is the difference between maximal and central squarefree
 *          kernel numbers dividing values of {binomial(n,k)} or A001405(n),
 *          respectively.
 * https://oeis.org/A048682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 34, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
/// OEIS A048682: a(n) is the difference between the maximal and
/// central square‑free kernel of \(\binom{n}{k}\) for 0≤k≤n.
/// The sequence is computed by iterating the binomial coefficients
/// and maintaining their square‑free kernels using a sieve of
/// smallest prime factors.
///
/// The function is fully reusable for the various integer types
/// specified by seq_t and idx_t.  It dynamically allocates a
/// smallest‑prime‑factor table up to the current argument and
/// reuses it across calls.  All arithmetic is performed in
/// seq_t and is exact as long as the value fits in that
/// representation, which is the requirement of the test harness.
**/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------------------------------------------------------- */
/*  Sieve for smallest prime factors.                         */
/* ---------------------------------------------------------- */

static idx_t _sieve_n = 0;
static uint32_t *_spf = NULL;     /* smallest prime factor for each integer */
static uint32_t *_prime_exp = NULL; /* current exponent of each prime in current binomial */

static void _build_sieve(idx_t n)
{
    if (n <= _sieve_n) return;    /* already available */

    /* free previous allocation */
    free(_spf);     free(_prime_exp);

    /* allocate new arrays */
    size_t sz = (size_t)(n + 1);
    _spf = (uint32_t *)malloc(sz * sizeof(uint32_t));
    _prime_exp = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!_spf || !_prime_exp) {
        _spf = _prime_exp = NULL;   /* signal error */
        return;
    }

    /* initialise SPF array */
    for (size_t i = 0; i <= n; ++i) {
        _spf[i] = (uint32_t)i;
        _prime_exp[i] = 0;
    }

    /* sieve */
    for (size_t p = 2; p * p <= (size_t)n; ++p) {
        if (_spf[p] == (uint32_t)p) {          /* p is prime */
            for (size_t j = p * p; j <= n; j += p) {
                if (_spf[j] == (uint32_t)j) {
                    _spf[j] = (uint32_t)p;
                }
            }
        }
    }
    _sieve_n = n;
}

/* ---------------------------------------------------------- */
/*  Update the current square‑free kernel while stepping through
 *  the row of Pascal’s triangle.
 *  m is the integer factor being multiplied (sign=+1) or divided
 *  (sign=-1).  curRad points to the product of distinct primes
 *  still present in the current binomial coefficient.
 * ---------------------------------------------------------- */
static void _factor_update(uint32_t m, int sign, seq_t *curRad)
{
    if (m <= 1) return;
    uint32_t x = m;
    while (x > 1) {
        uint32_t p = _spf[x];
        uint32_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        if (sign > 0) {
            if (_prime_exp[p] == 0) {
                *curRad *= (seq_t)p;
            }
            _prime_exp[p] += cnt;
        } else {   /* sign < 0 */
            _prime_exp[p] -= cnt;
            if (_prime_exp[p] == 0) {
                *curRad /= (seq_t)p;
            }
        }
    }
}

/* ---------------------------------------------------------- */
/*  Public function required by the harness.
 *  a(1) corresponds to n=1 in OEIS terminology.
 * ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* no negative indices; safe guard */

    _build_sieve(n);
    if (!_spf || !_prime_exp) return (seq_t)0;   /* failure to allocate */

    const idx_t central_k = n / 2;   /* floor(n/2) */
    bool centralSet = (central_k == 0);
    seq_t centralRad = (seq_t)1;     /* rad for binom(n,0) */

    seq_t curRad = (seq_t)1;         /* rad(current binomial) */
    seq_t maxRad  = (seq_t)1;        /* maximal rad seen so far */

    for (idx_t k = 0; k < n; ++k) {
        /* Transition from binom(n,k) to binom(n,k+1) */
        _factor_update((uint32_t)(n - k), +1, &curRad);  /* multiply by (n-k) */
        _factor_update((uint32_t)(k + 1), -1, &curRad);  /* divide by (k+1) */

        if (!centralSet && (k + 1) == central_k) {
            centralRad = curRad;
            centralSet = true;
        }

        if (curRad > maxRad) maxRad = curRad;
    }

    /* The central value is always set; in the unlikely event that
     * the maximal value is smaller, we return 0, which is the
     * zero difference. */
    if (maxRad < centralRad) return (seq_t)0;
    return maxRad - centralRad;
}
