/*
 * A176588  a(n) = A176558(A175354(n)) = numbers m as reverse concatenations
 *          of divisors of numbers from A175354, where A175354 = numbers k
 *          such that reverse concatenations of divisors of k are nonprimes.
 * https://oeis.org/A176588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reverse concatenation of the divisors of the n-th integer
 *            k such that this concatenation is non‑prime (k = A175354(n),
 *            a(n) = A176558(k))
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----- includes ----- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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

/* ----- helper: compare uint64_t for qsort ----- */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t va = *(const uint64_t *)a;
    const uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* ----- helper: compute all divisors of n (ascending order) ----- */
static void divisors_of_uint64(uint64_t n, uint64_t **out, size_t *cnt)
{
    /* Factor n. */
    uint64_t pr[16], ex[16];
    size_t pcnt = 0;
    factor_u64(n, pr, ex, &pcnt);

    /* Count number of divisors. */
    size_t divcnt = 1;
    for (size_t i = 0; i < pcnt; ++i)
        divcnt *= (ex[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(divcnt * sizeof(uint64_t));
    size_t pos = 1;            /* next free slot, divs[0] = 1 */
    divs[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = pr[i];
        uint64_t mult = 1;
        size_t start = pos;
        for (int e = 1; e <= ex[i]; ++e) {
            mult *= p;
            for (size_t j = 0; j < start; ++j)
                divs[pos++] = divs[j] * mult;
        }
    }

    /* Sort ascending. */
    qsort(divs, divcnt, sizeof(uint64_t), cmp_uint64);
    *out = divs;
    *cnt = divcnt;
}

/* ----- helper: reverse concatenation of divisors of k (descending order) ----- */
static seq_t rev_concat_uint64(uint64_t k)
{
    uint64_t *divs;
    size_t dcnt;
    divisors_of_uint64(k, &divs, &dcnt);

    seq_t result = 0;
    for (size_t i = dcnt; i > 0; --i) {
        uint64_t d = divs[i - 1];
        seq_t dd = (seq_t)d;            /* convert divisor to seq_t */
        int dl = num_digits(dd);         /* decimal digit count of dd */
        seq_t mult = pow10((idx_t)dl);
        result = result * mult + dd;
    }

    free(divs);
    return result;
}

/* ----- sequence function ----- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached a(k) values */
    static size_t cap   = 0;        /* allocated capacity */
    static idx_t  max_n = 0;        /* highest n computed so far */
    static uint64_t cur   = 1;      /* next integer to test */

    if (n < 1) return (seq_t)0;      /* undefined; keep harmless */

    if (n <= max_n)
        return cache[n - 1];

    /* Extend cache up to n. */
    while (max_n < n) {
        seq_t rc = rev_concat_uint64(cur);
        if (!is_prime_big(rc)) {
            /* store rc as a(max_n+1) */
            if (max_n + 1 > cap) {
                size_t new_cap = (cap == 0) ? 16 : (cap << 1);
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return (seq_t)0;         /* allocation failed */
                cache = tmp;
                cap   = new_cap;
            }
            cache[max_n] = rc;
            ++max_n;
        }
        ++cur;
    }
    return cache[n - 1];
}
