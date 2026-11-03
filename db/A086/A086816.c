/*
 * A086816  Smaller member of a twin prime pair with a triangular sum.
 * https://oeis.org/A086816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*A166119(n) - 1 Equivalently: a(n) is the least member p
 *            of a twin‑prime pair (p,p+2) such that 2*(p+1) is a triangular
 *            number: 4*(p+1) = t*(t+1) for some integer t
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Validated: standard, pass, 51 terms through n = 295 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Static storage for the terms of the sequence. */
static seq_t *terms = NULL;      /* array of found terms   */
static size_t terms_len = 0;     /* number of terms stored */
static size_t terms_cap = 0;     /* allocated capacity      */

/* Ensure that the terms array has at least <cap> slots. */
static void ensure_terms_capacity(size_t cap)
{
    if (terms_cap >= cap) return;
    size_t new_cap = terms_cap ? terms_cap : 16;
    while (new_cap < cap) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, terms, new_cap);
    terms = tmp;
    terms_cap = new_cap;
}

/* Generator state: next integer t to test (triangle parameter). */
static seq_t next_t = 8;   /* first t that yields a valid p is 8 (p = 17) */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n)
    {
        /* Compute t*(t+1). */
        seq_t tri = next_t * (next_t + 1);          /* t*(t+1) */
        seq_t num = tri - 4;                       /* numerator: t*(t+1) - 4 */

        /* Check divisibility by 4. */
        if (num % 4 != 0) {
            ++next_t;
            continue;  /* not an integer p */
        }

        /* Candidate prime p. */
        seq_t p = num / 4;

        /* Skip p <= 2 or even p to avoid trivial/non‑prime cases. */
        if (p < 3 || (p & 1) == 0) {
            ++next_t;
            continue;
        }

        /* Check primality of p and p+2. */
        if (is_prime_big(p) && is_prime_big(p + 2))
        {
            /* Valid term found. Store it. */
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = p;
        }

        ++next_t;   /* move to next t for next iteration */
    }

    return terms[(size_t)n - 1];
}
