/*
 * A198958  q-expansion of modular form psi_0^6/t_{3B}^2.
 * https://oeis.org/A198958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 175, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A198958
 * a(n) = coefficient of q^n in (η(q^3)^3 / η(q))^6,
 * i.e. a(0) = 0, a(1) = 0, a(2) = 1, a(3) = 6, a(4) = 27, ...
 * ------------------------------------------------------------------
 *  We compute the logarithm of the generating function,
 *  exponentiate it using the integer recurrence
 *
 *      f_0 = 1,
 *      f_n = (1/n) ∑_{k=1}^{n} S(k) f_{n-k}
 *
 *  where
 *      S(k) = ∑_{u|k} c(u)·u ,  c(u) = 6 if u≢0 (mod 3),  c(u) = –12 if u≡0 (mod 3).
 *
 *  The array f[n] equals the coefficient of q^(n+2) in the required series,
 *  so a(n) = f_{n-2} for n≥2.
 *
 *  All arithmetic is done with an unsigned, fixed‑base big integer
 *  (base = 10^9).  We only need addition, subtraction (a≥b),
 *  multiplication by a small unsigned integer, and division by a
 *  small unsigned integer.  The final value is converted to a
 *  decimal string and passed to the supplied seq_from_str().
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../utils/seq_from_str.h"

/* ------------------------------------------------------------ */
/*  Small fixed‑size big integer (base 1e9)                      */
/* ------------------------------------------------------------ */
#define BI_BASE   1000000000U
#define BI_MAXDIGITS 16          /* enough for >10^144  */

typedef struct {
    uint32_t d[BI_MAXDIGITS]; /* little‑endian limbs, base BI_BASE */
    size_t  len;              /* number of used limbs, 0 means 0 */
} BigInt;

static void bi_init(BigInt *a)          { a->len = 0; }
static void bi_set_uint(BigInt *a, uint32_t v) {
    if (v == 0) { a->len = 0; return; }
    a->len = 1; a->d[0] = v;
}

/* copy */
static void bi_copy(BigInt *dest, const BigInt *src) {
    dest->len = src->len;
    memcpy(dest->d, src->d, src->len * sizeof(uint32_t));
}

/* comparison: a - b, return >0 if a>b, 0 if equal, <0 if a<b */
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (int)(a->len) - (int)(b->len);
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i]) return (int)(a->d[i]) - (int)(b->d[i]);
    }
    return 0;
}

/* a += b */
static void bi_add(BigInt *a, const BigInt *b) {
    size_t ml = a->len > b->len ? a->len : b->len;
    uint64_t carry = 0;
    for (size_t i = 0; i < ml; ++i) {
        uint64_t av = (i < a->len) ? a->d[i] : 0;
        uint64_t bv = (i < b->len) ? b->d[i] : 0;
        uint64_t sum = av + bv + carry;
        a->d[i] = (uint32_t)(sum % BI_BASE);
        carry    = sum / BI_BASE;
    }
    if (carry) {
        if (ml < BI_MAXDIGITS) {
            a->d[ml] = (uint32_t)carry;
            ml = ml + 1;
        }
    }
    a->len = ml;
    /* trim leading zeros */
    while (a->len > 0 && a->d[a->len - 1] == 0) a->len--;
}

/* a -= b   (assumes a >= b) */
static void bi_sub(BigInt *a, const BigInt *b) {
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t av = a->d[i];
        uint64_t bv = (i < b->len) ? b->d[i] : 0;
        int64_t diff = (int64_t)av - bv - (int64_t)borrow;
        if (diff < 0) {
            diff += BI_BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (uint32_t)diff;
    }
    /* trim leading zeros */
    while (a->len > 0 && a->d[a->len - 1] == 0) a->len--;
}

/* a *= m   (m fits in 32 bits) */
static void bi_mul_small(BigInt *a, uint32_t m) {
    if (m == 0 || a->len == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BI_BASE);
        carry   = prod / BI_BASE;
    }
    while (carry) {
        if (a->len < BI_MAXDIGITS) {
            a->d[a->len++] = (uint32_t)(carry % BI_BASE);
            carry /= BI_BASE;
        } else {
            /* overflow – should not happen for our problem limits */
            break;
        }
    }
}

/* a /= d   (d fits in 32 bits, division is exact) */
static void bi_div_small(BigInt *a, uint32_t d) {
    uint64_t rem = 0;
    for (size_t i = a->len; i-- > 0;) {
        uint64_t cur = rem * BI_BASE + a->d[i];
        a->d[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    /* trim leading zeros */
    while (a->len > 0 && a->d[a->len - 1] == 0) a->len--;
}

/* convert BigInt to decimal string (max 64 chars) */
static void bi_to_string(const BigInt *a, char *buf, size_t bufsize) {
    if (a->len == 0) {
        strncpy(buf, "0", bufsize);
        return;
    }
    size_t pos = 0;
    pos += snprintf(buf + pos, bufsize - pos, "%u", a->d[a->len - 1]);
    for (size_t i = a->len - 1; i-- > 0; ) {
        snprintf(buf + pos, bufsize - pos, "%09u", a->d[i]);
        pos = strlen(buf);
    }
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2) {
        /* a(0) and a(1) are zero */
        return (seq_t)0;
    }

    /* We need coefficients up to index N = n-2 (shift by q^2) */
    const idx_t N = n - 2;
    const size_t Nsize = (size_t)N + 1;

    /* Pre‑compute c(u) and S(k) for k up to N */
    int32_t *c = (int32_t*)calloc(Nsize, sizeof(int32_t));
    int64_t *S = (int64_t*)calloc(Nsize, sizeof(int64_t));
    if (!c || !S) {
        free(c); free(S);
        return (seq_t)0;
    }
    for (size_t u = 1; u <= N; ++u) {
        c[u] = (int32_t)((u % 3 == 0) ? -12 : 6);
    }
    /* S[m] = sum over divisors u of m: c[u] * u */
    for (size_t u = 1; u <= N; ++u) {
        int32_t coeff = c[u];
        for (size_t m = u; m <= N; m += u) {
            S[m] += (int64_t)coeff * (int64_t)u;
        }
    }

    /* Allocate array f[0..N] of BigInt */
    BigInt *f = (BigInt*)calloc(Nsize, sizeof(BigInt));
    if (!f) {
        free(c); free(S);
        return (seq_t)0;
    }
    for (size_t i = 0; i < Nsize; ++i) bi_init(&f[i]);

    /* f[0] = 1 */
    bi_set_uint(&f[0], 1);

    /* Temporary big integers for computation */
    BigInt sum, neg, temp;
    /* Compute f[n] for n=1..N using recurrence */
    for (size_t nidx = 1; nidx <= N; ++nidx) {
        bi_init(&sum);
        bi_init(&neg);
        /* inner loop over k */
        for (size_t k = 1; k <= nidx; ++k) {
            int64_t coeff = S[k];
            if (coeff == 0) continue;

            bi_copy(&temp, &f[nidx - k]);          /* temp = f[n-k] */
            bi_mul_small(&temp, (uint32_t)(coeff > 0 ? coeff : -coeff));

            if (coeff > 0) {
                bi_add(&sum, &temp);
            } else {
                bi_add(&neg, &temp);
            }
        }
        /* sum >= neg is guaranteed by the combinatorial formula */
        bi_sub(&sum, &neg);            /* sum = sum - neg */
        bi_div_small(&sum, (uint32_t)nidx); /* sum /= nidx */
        bi_copy(&f[nidx], &sum);       /* f[nidx] = sum */
    }

    /* Convert f[N] (which equals a(n)) to seq_t */
    char buf[64];
    bi_to_string(&f[N], buf, sizeof(buf));
    seq_t result = seq_from_str(buf);

    /* Free allocated memory */
    free(c); free(S); free(f);

    return result;
}
