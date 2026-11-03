/*
 * A082654  Order of 4 mod n-th prime: least k such that prime(n) divides
 *          4^k-1, n >= 2.
 * https://oeis.org/A082654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, if n = 1; otherwise a(n) = order of 4 modulo prime(n)
 *            (least k ≥ 1 such that 4^k ≡ 1 (mod prime(n)))
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 960 (2026-11-03)
 * Helpers:   factorize, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- required helpers from utils ---------------------------------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factorize.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* --- helper: enumerate all divisors of a number -------------------------- */
static void
gen_divs(int idx, seq_t cur, uint64_t *primes, int *exps, int cnt,
         seq_t *out, size_t *outcnt)
{
    if (idx == cnt) {
        out[(*outcnt)++] = cur;
        return;
    }
    seq_t val = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divs(idx + 1, cur * val, primes, exps, cnt, out, outcnt);
        val *= primes[idx];
    }
}

/* --- main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* nth prime (n ≥ 2) */
    uint32_t p32 = get_prime(n);
    uint64_t p   = (uint64_t)p32;

    /* φ(p) = p - 1 */
    uint64_t phi = p - 1;

    /* factor φ */
    uint64_t fac[64];
    int      exp[64];
    size_t   faccnt = factorize(phi, fac, exp);

    /* generate all divisors of φ */
    seq_t divs[2048];   /* enough for φ < 2^32 */
    size_t dcnt = 0;
    gen_divs(0, 1, fac, exp, (int)faccnt, divs, &dcnt);

    /* sort divisors ascending */
    mem_qsort(divs, dcnt, sizeof(seq_t), cmp_seq_t);

    /* find the smallest divisor d with 4^d ≡ 1 (mod p) */
    for (size_t i = 0; i < dcnt; ++i) {
        seq_t d = divs[i];
        if (powmod((seq_t)4, (uint64_t)d, (seq_t)p) == (seq_t)1)
            return d;
    }

    /* for odd primes this should never happen */
    return (seq_t)0;
}
