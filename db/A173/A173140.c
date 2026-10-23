/*
 * A173140  Numbers k such that the period of the decimal representation of
 *          1/k is a square.
 * https://oeis.org/A173140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A173140 – numbers k such that the period of 1/k is a square.
 *            This implementation lazily generates the sequence and caches the
 *            first n terms seen. No external helper functions are required
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Local helper:  factorization of a 64‑bit integer.           */
/* ------------------------------------------------------------ */
static void factor_u64_local(uint64_t n,
                             uint64_t *pr, uint64_t *ex,
                             size_t *cnt)
{
    size_t pcnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[pcnt] = p;
            ex[pcnt] = e;
            ++pcnt;
        }
    }
    if (n > 1) {
        pr[pcnt] = n;
        ex[pcnt] = 1;
        ++pcnt;
    }
    *cnt = pcnt;
}

/* ------------------------------------------------------------ */
/*  Local helper:  binary modular exponentiation.               */
/* ------------------------------------------------------------ */
static seq_t modpow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* ------------------------------------------------------------ */
/*  Local helper:  floor square root of a seq_t integer.        */
/* ------------------------------------------------------------ */
static seq_t sqrt_floor(seq_t x)
{
    seq_t lo = 0, hi = x;
    while (lo <= hi) {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x) {
            if (mid2 == x) return mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* ------------------------------------------------------------ */
/*  The sequence accessor.                                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Cache of already found terms. */
    static seq_t *terms    = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    /* Next candidate k to examine.  Begin at the smallest odd, non‑5 multiple. */
    static uint64_t next_k = 3;

    if (n == 0) return (seq_t)0;   /* Undefined for the built‑in offset. */

    /* Grow the cache until it contains at least n terms. */
    while (terms_len < (size_t)n) {
        /* Skip numbers not coprime to 10. */
        if ((next_k & 1ULL) == 0ULL || (next_k % 5ULL) == 0ULL) {
            next_k++;
            continue;
        }

        uint64_t k64 = next_k;
        seq_t   k = (seq_t)k64;

        /* ---- factor k ---- */
        uint64_t primes[64], exps[64];
        size_t pcnt = 0;
        factor_u64_local(k64, primes, exps, &pcnt);

        /* ---- compute φ(k) ---- */
        seq_t phi = k;
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t p = primes[i];
            phi = (phi / (seq_t)p) * (seq_t)(p - 1);
        }

        /* ---- multiplicative order of 10 modulo k ---- */
        uint64_t ord = (uint64_t)phi;   /* φ(k) fits into 64 bits */
        bool reduced;
        do {
            reduced = false;
            /* factor the current order */
            uint64_t o_pr[64], o_ex[64];
            size_t o_cnt = 0;
            factor_u64_local(ord, o_pr, o_ex, &o_cnt);

            for (size_t j = 0; j < o_cnt; ++j) {
                uint64_t q = o_pr[j];
                while (ord % q == 0) {
                    uint64_t cand = ord / q;
                    if (modpow((seq_t)10, cand, k) == (seq_t)1) {
                        ord = cand;
                        reduced = true;
                    } else {
                        break;
                    }
                }
            }
        } while (reduced);

        /* ---- test whether the period is a perfect square ---- */
        seq_t period = (seq_t)ord;
        seq_t sq = sqrt_floor(period);
        if (sq * sq == period) {
            /* Append k to the cache. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* Out of memory */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = k;
        }

        next_k++;
    }

    return terms[n - 1];
}
