/*
 * A055567  Sum of digits of n^6.
 * https://oeis.org/A055567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of decimal digits of n^6
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Big‑integer base and space */
#define BI_BASE 1000000000u
#define BI_MAX  32

/* Multiply two big integers a * b.
   a[0..alen-1] and b[0..blen-1] are little‑endian words in BI_BASE.
   Result is written into c[0..] and its length in clen. */
static void bi_mul(const uint32_t *a, size_t alen,
                   const uint32_t *b, size_t blen,
                   uint32_t *c, size_t *clen)
{
    size_t i, j;
    size_t L = alen + blen;
    for (i = 0; i < L; ++i) c[i] = 0;
    for (i = 0; i < alen; ++i) {
        uint64_t carry = 0;
        for (j = 0; j < blen; ++j) {
            uint64_t t = (uint64_t)a[i] * b[j] + c[i + j] + carry;
            c[i + j] = (uint32_t)(t % BI_BASE);
            carry = t / BI_BASE;
        }
        c[i + blen] = (uint32_t)carry;
    }
    *clen = L;
    while (*clen > 1 && c[*clen - 1] == 0) --(*clen);
}

/* Convert a uint64_t value into a little‑endian big integer. */
static void uint64_to_bi(uint64_t v, uint32_t *d, size_t *len)
{
    if (v == 0) {
        d[0] = 0;
        *len = 1;
        return;
    }
    size_t i = 0;
    while (v) {
        d[i++] = (uint32_t)(v % BI_BASE);
        v /= BI_BASE;
    }
    *len = i;
}

seq_t a(idx_t n)
{
    /* Build n^6 in base BI_BASE */
    uint32_t base[BI_MAX];
    uint32_t result[BI_MAX];
    size_t base_len, result_len;

    uint64_to_bi((uint64_t)n, base, &base_len);

    /* Start with 1 */
    result[0] = 1;
    result_len = 1;

    for (int i = 0; i < 6; ++i) {
        uint32_t tmp[BI_MAX];
        size_t tmp_len;
        bi_mul(result, result_len, base, base_len, tmp, &tmp_len);
        /* copy back to result */
        for (size_t k = 0; k < tmp_len; ++k) result[k] = tmp[k];
        result_len = tmp_len;
    }

    /* Digit sum */
    seq_t sum = 0;
    for (size_t i = 0; i < result_len; ++i) {
        uint32_t v = result[i];
        while (v) {
            sum += v % 10;
            v /= 10;
        }
    }
    return sum;
}
