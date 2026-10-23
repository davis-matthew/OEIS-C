/*
 * A146984  List of integer-valued contraharmonic means (u^2+v^2)/(u+v) of two
 *          positive integers u and v (with u < v) ordered by increasing u and
 *          increasing v (u = 2, 3, 4, ...; v = u+1, u+2, ...).
 * https://oeis.org/A146984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = contraharmonic mean (u^2+v^2)/(u+v) for all pairs (u,v)
 *            with 2 ≤ u < v, ordered by increasing u then increasing v
 * Range:     u8 n <= 33, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define SPF_MAX 500000       /* maximum u handled in this implementation */

static uint32_t *spf = NULL;

/* Simple sieve for least prime factors up to SPF_MAX */
static void init_spf(void) {
    if (spf) return;
    spf = (uint32_t *)malloc((SPF_MAX + 1) * sizeof(uint32_t));
    for (idx_t i = 0; i <= SPF_MAX; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= SPF_MAX; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            if (i * i <= SPF_MAX) {
                for (idx_t j = i * i; j <= SPF_MAX; j += i)
                    if (!spf[j]) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* Comparator for qsort of uint64_t */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t aa = *(const uint64_t *)a;
    uint64_t bb = *(const uint64_t *)b;
    return (aa < bb) ? -1 : (aa > bb);
}

/* Recursive enumeration of divisors of 2*u^2 */
static void enumerate_divisors(const uint32_t *primes, const uint8_t *exps,
                               size_t count, uint64_t cur,
                               uint64_t *buf, size_t *pos) {
    if (count == 0) {
        buf[(*pos)++] = cur;
        return;
    }
    uint32_t p = primes[count - 1];
    uint8_t e = exps[count - 1];
    enumerate_divisors(primes, exps, count - 1, cur, buf, pos);
    for (uint8_t k = 0; k < e; ++k) {
        cur *= p;
        enumerate_divisors(primes, exps, count - 1, cur, buf, pos);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n=0 */

    init_spf();

    idx_t count = 0;
    idx_t u = 1;

    /* Buffers for divisor enumeration */
    uint64_t div_buf[2000];
    size_t div_cnt;

    while (1) {
        ++u;                     /* start from u = 2 */
        if (u > SPF_MAX) {
            /* increase SPF table if needed (unlikely for n <= 10000) */
            /* simple re-sieve with double size */
            size_t old = SPF_MAX;
            size_t new_max = old * 2;
            spf = (uint32_t *)realloc(spf, (new_max + 1) * sizeof(uint32_t));
            for (idx_t i = old + 1; i <= new_max; ++i) spf[i] = 0;
            for (idx_t i = 2; i <= new_max; ++i) {
                if (!spf[i]) {
                    spf[i] = (uint32_t)i;
                    if (i * i <= new_max) {
                        for (idx_t j = i * i; j <= new_max; j += i)
                            if (!spf[j]) spf[j] = (uint32_t)i;
                    }
                }
            }
        }

        /* Factorize u */
        uint32_t primes_tmp[64];
        uint8_t exp_tmp[64];
        size_t cnt_tmp = 0;
        idx_t temp = u;
        while (temp > 1) {
            uint32_t p = spf[temp];
            uint8_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes_tmp[cnt_tmp] = p;
            exp_tmp[cnt_tmp] = e;
            ++cnt_tmp;
        }

        /* Build prime list for 2*u^2 */
        uint32_t primes[64];
        uint8_t exps[64];
        size_t cnt = 0;
        int has2 = 0;
        for (size_t i = 0; i < cnt_tmp; ++i) {
            if (primes_tmp[i] == 2) {
                exps[cnt] = 2 * exp_tmp[i] + 1;
                primes[cnt] = 2;
                has2 = 1;
            } else {
                exps[cnt] = 2 * exp_tmp[i];
                primes[cnt] = primes_tmp[i];
            }
            ++cnt;
        }
        if (!has2) {
            primes[cnt] = 2;
            exps[cnt] = 1;       /* exponent for prime 2 */
            ++cnt;
        }

        /* Enumerate divisors of 2*u^2 */
        div_cnt = 0;
        enumerate_divisors(primes, exps, cnt, 1ULL, div_buf, &div_cnt);
        qsort(div_buf, div_cnt, sizeof(uint64_t), cmp_uint64);

        /* Process divisors > 2*u */
        for (size_t i = 0; i < div_cnt; ++i) {
            uint64_t d = div_buf[i];
            if (d <= 2 * (uint64_t)u) continue;  /* ensures v > u */
            idx_t v = (idx_t)(d - u);
            uint64_t num = (uint64_t)u * (uint64_t)u + (uint64_t)v * (uint64_t)v;
            seq_t value = (seq_t)(num / d);
            ++count;
            if (count == n) return value;
        }
    }
}
