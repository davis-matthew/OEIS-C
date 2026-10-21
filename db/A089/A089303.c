/*
 * A089303  a(n) = floor( (10^n - 1) / (9*n) ).
 * https://oeis.org/A089303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((10^n - 1)/(9*n))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* ---- Small arbitrary‑precision unsigned integer (base 1e9) ---- */
#define BIGBASE 1000000000U
#define BIGMAXDIG 3000          /* enough to hold 10^10000 */

typedef struct {
    uint32_t d[BIGMAXDIG];
    size_t   len;              /* number of used digits */
} BigUInt;

/* initialise BigUInt to an unsigned 64‑bit value */
static void big_init_u64(BigUInt *x, uint64_t v)
{
    x->len = 0;
    while (v) {
        x->d[x->len++] = v % BIGBASE;
        v /= BIGBASE;
    }
    /* zero stays with len==0 */
}

/* copy src to dst */
static void big_copy(BigUInt *dst, const BigUInt *src)
{
    dst->len = src->len;
    if (src->len) {
        for (size_t i = 0; i < src->len; ++i)
            dst->d[i] = src->d[i];
    }
}

/* multiply src by a small integer m (≤ UINT64_MAX) → dst */
static void big_mul_small(BigUInt *dst, const BigUInt *src, uint64_t m)
{
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < src->len; ++i) {
        uint64_t prod = (uint64_t)src->d[i] * m + carry;
        dst->d[i]      = prod % BIGBASE;
        carry          = prod / BIGBASE;
    }
    while (carry) {
        dst->d[i++] = carry % BIGBASE;
        carry       = carry / BIGBASE;
    }
    dst->len = i;
}

/* add a small integer to BigUInt in place */
static void big_add_small(BigUInt *x, uint64_t add)
{
    uint64_t carry = add;
    size_t i = 0;
    while (carry) {
        if (i < x->len) {
            uint64_t sum = (uint64_t)x->d[i] + carry;
            x->d[i]      = sum % BIGBASE;
            carry        = sum / BIGBASE;
            ++i;
        } else {
            x->d[i++] = carry % BIGBASE;
            carry     = carry / BIGBASE;
        }
    }
    if (i > x->len)
        x->len = i;
}

/* divide src by small divisor → quotient in quot, remainder in rem */
static void big_div_small(const BigUInt *src, uint64_t divisor,
                          BigUInt *quot, uint64_t *rem)
{
    quot->len = src->len;
    uint64_t residue = 0;
    for (int i = (int)src->len - 1; i >= 0; --i) {
        uint64_t cur = src->d[i] + residue * BIGBASE;
        quot->d[i]   = cur / divisor;
        residue      = cur % divisor;
    }
    while (quot->len > 0 && quot->d[quot->len - 1] == 0)
        --quot->len;
    *rem = residue;
}

/* convert BigUInt to the build‑specific seq_t type */
static seq_t big_to_seq(const BigUInt *x)
{
    seq_t val = 0;
    for (int i = (int)x->len - 1; i >= 0; --i) {
        val = val * (seq_t)BIGBASE + (seq_t)x->d[i];
    }
    return val;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* below the OEIS offset */

    BigUInt A, nextA, temp;
    big_init_u64(&A, 1);              /* a(1) = 1 */
    uint64_t r = 0;                   /* remainder of repunit / current k */

    for (idx_t k = 1; k < n; ++k) {
        /* temp = 10*k*A + 10*r + 1 */
        uint64_t mul = 10ULL * (uint64_t)k;
        big_mul_small(&temp, &A, mul);
        big_add_small(&temp, 10ULL * r);
        big_add_small(&temp, 1ULL);

        /* divide temp by k+1 → nextA, r */
        big_div_small(&temp, (uint64_t)(k + 1), &nextA, &r);

        /* prepare for next iteration */
        big_copy(&A, &nextA);
    }

    return big_to_seq(&A);
}
