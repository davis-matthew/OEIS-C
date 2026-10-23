/*
 * A168157  Number of 0's in the matrix whose lines are the binary expansion
 *          of the first n primes.
 * https://oeis.org/A168157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n*A035100(n) - A095375(n)
 * Range:     u8 n <= 54, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A035100(n) = bit length of the nth prime */
#include <stdint.h>
#include <stdlib.h>

/* Helper: number of bits required to represent a non‑zero 64‑bit integer. */
static unsigned bitlen_uint64(uint64_t v)
{
    unsigned len = 0;
    while (v) {
        len++;
        v >>= 1;
    }
    return len;
}

/* ------------------------------------------------------------------ */
/*  Prime generation and caching                                       */
/* ------------------------------------------------------------------ */

/* Stored primes (1‑based in the calling code).  The array grows as needed. */
static uint64_t *primes = NULL;
/* Sum of popcounts of all primes up to the current count (pref[0]==0).  */
static uint64_t *pref   = NULL;
static idx_t prime_cnt  = 0;   /* number of primes currently stored */
static idx_t prime_cap  = 0;   /* allocated capacity for primes array   */

/* ensure_primes makes sure that at least `need` primes are generated. */
static void ensure_primes(idx_t need)
{
    if (need <= prime_cnt)
        return;

    /* Expand capacity if needed.  Double until big enough. */
    if (prime_cap < need) {
        idx_t new_cap = prime_cap ? prime_cap * 2 : 1;
        while (new_cap < need)
            new_cap <<= 1;
        uint64_t *tmp_pr = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp_pr)
            abort();
        primes = tmp_pr;
        uint64_t *tmp_pref = (uint64_t *)realloc(pref, (new_cap + 1) * sizeof(uint64_t));
        if (!tmp_pref)
            abort();
        pref = tmp_pref;
        prime_cap = new_cap;
    }

    /* Generate primes up to the required count.  Trial division is fast for
       primes below 100 000 (the 10 000th prime). */
    uint64_t cand = prime_cnt ? primes[prime_cnt - 1] + 1 : 2;

    while (prime_cnt < need) {
        if (cand > 2 && (cand & 1ULL) == 0) {
            cand++;
            continue;
        }
        int isprime = 1;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                isprime = 0;
                break;
            }
        }
        if (isprime) {
            primes[prime_cnt] = cand;
            pref[prime_cnt + 1] = pref[prime_cnt] + __builtin_popcountll(cand);
            ++prime_cnt;
        }
        ++cand;
    }
}

/* ------------------------------------------------------------------ */
/*  Sequence function                                                   */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1, but return 0 for safety */

    ensure_primes(n);               /* make sure we have the first n primes */
    uint64_t p = primes[n - 1];     /* nth prime (1‑based) */
    unsigned L = bitlen_uint64(p);  /* A035100(n) */
    seq_t total_ones = (seq_t)pref[n];          /* A095375(n) */
    seq_t result = (seq_t)n * (seq_t)L - total_ones;
    return result;
}
