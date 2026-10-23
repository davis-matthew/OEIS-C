/*
 * A048684  Multiplicity of the maximum squarefree kernel function applied to
 *          the binomial coefficients C(n,k).
 * https://oeis.org/A048684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   traditional computation of a(n) using prime factor updates and
 *            big integer arithmetic for the square‑free kernel of each
 *            binomial coefficient C(n,k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- Big integer on base 1e9 ------------------------------------*/
#define BIGINT_BASE 1000000000U

typedef struct {
    size_t len;      /* used limbs (least‑significant first) */
    size_t cap;      /* allocated limbs                */
    uint32_t *limb;  /* limbs                           */
} bigint;

static void bi_init(bigint *b, uint32_t val) {
    b->len = 1;
    b->cap = 4;
    b->limb = (uint32_t*)malloc(b->cap * sizeof(uint32_t));
    b->limb[0] = val;
}

static void bi_ensure(bigint *b, size_t need) {
    if (b->cap < need) {
        size_t newcap = b->cap * 2;
        while (newcap < need) newcap <<= 1;
        b->limb = (uint32_t*)realloc(b->limb, newcap * sizeof(uint32_t));
        b->cap = newcap;
    }
}

static void bi_free(bigint *b) {
    free(b->limb);
}

static void bi_mul_small(bigint *b, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->limb[i] * m + carry;
        b->limb[i] = (uint32_t)(prod % BIGINT_BASE);
        carry = prod / BIGINT_BASE;
    }
    while (carry) {
        bi_ensure(b, b->len + 1);
        b->limb[b->len++] = (uint32_t)(carry % BIGINT_BASE);
        carry /= BIGINT_BASE;
    }
}

static void bi_div_small(bigint *b, uint32_t d) {
    uint64_t rem = 0;
    for (size_t i = b->len; i-- > 0; ) {
        uint64_t cur = rem * BIGINT_BASE + b->limb[i];
        b->limb[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    while (b->len > 1 && b->limb[b->len - 1] == 0) --b->len;
}

static void bi_copy(bigint *dst, const bigint *src) {
    bi_ensure(dst, src->len);
    for (size_t i = 0; i < src->len; ++i) dst->limb[i] = src->limb[i];
    dst->len = src->len;
}

static int bi_cmp(const bigint *a, const bigint *b) {
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->limb[i] != b->limb[i])
            return (a->limb[i] > b->limb[i]) ? 1 : -1;
    }
    return 0;
}

/* ---------- OEIS A048684 ----------------------------------------------*/
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    /* smallest prime factor sieve */
    uint32_t *spf = (uint32_t*)malloc((size_t)(n + 1) * sizeof(uint32_t));
    int    *prime_idx = (int*)malloc((size_t)(n + 1) * sizeof(int));
    uint32_t *primes = (uint32_t*)malloc((size_t)(n + 1) * sizeof(uint32_t));
    if (!spf || !prime_idx || !primes) {
        free(spf); free(prime_idx); free(primes);
        return (seq_t)0;
    }

    for (idx_t i = 0; i <= n; ++i) {
        spf[i] = (uint32_t)i;
        prime_idx[i] = -1;
    }
    for (uint32_t i = 2; (uint64_t)i * i <= (uint64_t)n; ++i) {
        if (spf[i] == i) {
            for (uint32_t j = i * i; j <= (uint32_t)n; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    size_t primes_cnt = 0;
    for (uint32_t i = 2; i <= (uint32_t)n; ++i) {
        if (spf[i] == i) {
            primes[primes_cnt] = i;
            prime_idx[i] = (int)primes_cnt;
            ++primes_cnt;
        }
    }

    /* exponent array for current binomial coefficient */
    uint16_t *exp = (uint16_t*)calloc(primes_cnt, sizeof(uint16_t));
    if (!exp) {
        free(spf); free(prime_idx); free(primes);
        return (seq_t)0;
    }

    /* big integer rad and max_rad */
    bigint rad, max_rad;
    bi_init(&rad, 1);
    bi_init(&max_rad, 1);

    size_t max_cnt = 1;            /* k = 0 already counted */

    for (idx_t k = 1; k <= n; ++k) {
        /* factor (n-k+1) : multiply */
        uint32_t a = (uint32_t)(n - k + 1);
        uint32_t tmp = a;
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            int idx = prime_idx[p];
            if (exp[idx] == 0) bi_mul_small(&rad, p);
            ++exp[idx];
            tmp /= p;
        }

        /* factor k : divide */
        uint32_t b = (uint32_t)k;
        tmp = b;
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            int idx = prime_idx[p];
            if (exp[idx] == 1) bi_div_small(&rad, p);
            --exp[idx];
            tmp /= p;
        }

        int cmp = bi_cmp(&rad, &max_rad);
        if (cmp > 0) {
            bi_copy(&max_rad, &rad);
            max_cnt = 1;
        } else if (cmp == 0) {
            ++max_cnt;
        }
    }

    bi_free(&rad);
    bi_free(&max_rad);
    free(spf); free(prime_idx); free(primes); free(exp);

    return (seq_t)max_cnt;
}
