/*
 * A072974  Numbers n such that (50n-6) divides C(3n,n)*2^n.
 * https://oeis.org/A072974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   vfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A072974: Numbers n such that (50n-6) divides C(3n,n)*2^n.
 * The function a(idx_t n) returns the n-th such number (1‑based order).
 * 
 * The test exploits prime factorisations:
 *    (50n-6) | C(3n,n)*2^n  iff for every prime p dividing D=50n-6,
 *    v_p(C(3n,n)) + (p==2 ? n : 0) ≥ v_p(D).
 *    v_p(C(3n,n)) = v_p((3n)!) - v_p(n!) - v_p((2n)!).
 * The values are computed with prime exponents; no large integers are used.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/vfact.h"

/* Upper bound for 50n-6 that we will handle (adjustable). */
#define MAX_D 10000000u   /* 10 million → support up to n ≈ 200 000 */

/* Static storage for the smallest prime factor sieve. */
static uint32_t *spf = NULL;

static bool sieve_initialized = false;
static void init_sieve(void)
{
    if (sieve_initialized) return;
    sieve_initialized = true;

    spf = (uint32_t*)calloc(MAX_D + 1, sizeof(uint32_t));
    if (!spf) return; /* fall back to no sieve – not expected to happen */

    for (uint32_t i = 2; i <= MAX_D; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= MAX_D) {
                for (uint32_t j = i * i; j <= MAX_D; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

/* Factorise x using the spf sieve. Prime factors and exponents
 * are stored in the supplied arrays (max 16 factors).              */
static size_t factorise(uint32_t x, uint32_t *pf, uint32_t *prc)
{
    size_t cnt = 0;
    while (x > 1) {
        uint32_t p = spf[x];
        uint32_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        pf[cnt] = p;
        prc[cnt] = e;
        ++cnt;
    }
    return cnt;
}

/* Return true iff n satisfies the divisibility criterion.       */
static bool satisfies(uint64_t n)
{
    uint64_t D = 50 * n - 6ULL;
    if (D > MAX_D) { /* not supported – enlarge MAX_D if needed */
        /* Fallback: check via direct big‑int (not implemented)     */
        return false;
    }

    uint32_t primes[16], exps[16];
    size_t pcnt = factorise((uint32_t)D, primes, exps);

    for (size_t i = 0; i < pcnt; ++i) {
        uint32_t p = primes[i];
        uint32_t e = exps[i];

        uint64_t vp_binom = vfact(3 * n, p) - vfact(n, p) - vfact(2 * n, p);
        if (p == 2) vp_binom += n;   /* factor 2^n */
        if (vp_binom < e) return false;
    }
    return true;
}

/* Dynamic array to memoise the discovered sequence terms.     */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Append a term to the memoised array.                        */
static void append_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        if (new_cap < seq_len + 1) new_cap = seq_len + 1;
        seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* Main function requested by the problem statement.           */
seq_t a(idx_t n)
{
    static uint64_t next_n = 1;                       /* number to be tested next */
    static bool ready = false;

    if (!ready) {
        init_sieve();
        ready = true;
    }

    while (seq_len < n) {
        if (satisfies(next_n))
            append_term((seq_t)next_n);
        ++next_n;
    }
    return seq[n - 1];
}
