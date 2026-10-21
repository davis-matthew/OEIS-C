/*
 * A081945  Numbers k such that both k*(k + 1) + 1 and k*(k - 1) + 1 are
 *          primes.
 * https://oeis.org/A081945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 492, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: A081945 – numbers k such that both k*(k+1)+1 and k*(k-1)+1
    are prime.  This implementation searches forward from k=2, testing
    each candidate with a deterministic 64‑bit Miller–Rabin primality
    routine.  The found values are memoised in a static table so that
    calls to a(n) with arbitrary ordering are fast. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*               Deterministic Miller–Rabin for 64‑bit numbers          */
/* ------------------------------------------------------------------ */
static bool _is_prime(uint64_t n)
{
    if (n < 2)            return false;
    static const uint64_t small[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small/sizeof *small; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    /* n - 1 = d * 2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof bases/sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;

        __uint128_t x = 1, base = a, exp = d;
        /* modular exponentiation */
        while (exp) {
            if (exp & 1) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*                              Main routine                          */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;        /* memoised a(n) values */
static size_t  cache_cap = 0;      /* capacity of cache array  */
static idx_t   cache_len = 0;      /* number of terms stored  */
static uint64_t next_k = 2;        /* next candidate k to test */

/* a(1) -> first term, a(2) -> second term, …                 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n<1              */

    /* Initialise cache lazily */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;    /* allocation failure */
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t cand = next_k;

        /* compute k*(k±1)+1, safe with 128‑bit intermediate */
        __uint128_t v1_128 = (__uint128_t)cand * (cand + 1ULL) + 1ULL;
        __uint128_t v2_128 = (__uint128_t)cand * (cand - 1ULL) + 1ULL;

        /* cast to 64‑bit; numbers are far below 2^64 for the required range */
        uint64_t v1 = (uint64_t)v1_128;
        uint64_t v2 = (uint64_t)v2_128;

        if (_is_prime(v1) && _is_prime(v2)) {
            /* Found a valid k */
            /* Grow cache if necessary */
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (tmp == NULL) return 0;      /* allocation failure */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)cand;
            ++next_k;   /* advance to next candidate */
        } else {
            ++next_k;   /* try next k */
        }
    }

    return cache[n - 1];
}
