/*
 * A025692  Index of 2^n within the sequence of the numbers of the form
 *          2^i*6^j.
 * https://oeis.org/A025692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=1..n} ceil(k*log_6(2))
 * Range:     u8 n <= 34, u16 n <= 580, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses an iterative routine that keeps the running
   values of 2^k and 6^j (j = ceil(k*log_6(2)) ) as small
   big‑integers represented by a fixed‑length array of 32‑bit
   limbs.  All arithmetic on the result is performed in the
   seq_t type supplied by the build.  The routine works for
   any idx_t (uint64_t) and any seq_t (small integer, 64‑bit,
   or arbitrary precision). */

#include <stdint.h>
#include <stddef.h>

/* Big‑integer representation ------------------------------------------- */

#define BIG_CAP 512            /* enough for 2^10000 and 6^4000 */
typedef struct {
    uint32_t d[BIG_CAP];       /* little‑endian limbs, base 2^32 */
    size_t len;                /* current number of limbs */
} BigInt;

static void big_init(BigInt *x)
{
    x->len = 1;
    x->d[0] = 1;
}

static void big_mul_small(BigInt *x, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t t = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) {
        x->d[x->len++] = (uint32_t)carry;
    }
}

/* Compare a and b.
   Return 1 if a > b, 0 if equal, -1 if a < b. */
static int big_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len)
        return (a->len > b->len) ? 1 : -1;

    for (int i = (int)a->len - 1; i >= 0; --i) {
        if (a->d[i] != b->d[i])
            return (a->d[i] > b->d[i]) ? 1 : -1;
    }
    return 0;
}

/* Main sequence function ---------------------------------------------- */

seq_t a(idx_t n)
{
    /* a(0) = 1 directly */
    if (n == 0)
        return 1;

    BigInt p2, p6;
    big_init(&p2);             /* represents 2^k, starts at 1 */
    big_init(&p6);             /* represents 6^j, starts at 1 */

    int ceil_val = 0;          /* j = ceil(k*log_6(2)) starts at 0 */
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* 2^k := 2^(k-1) * 2 */
        big_mul_small(&p2, 2);

        /* Increase j until 6^j >= 2^k */
        while (big_cmp(&p2, &p6) > 0) {
            big_mul_small(&p6, 6);
            ++ceil_val;
        }

        /* Accumulate the ceil value */
        sum += (seq_t)ceil_val;
    }

    return sum + 1;   /* final a(n) = 1 + sum_{k=1..n} ceil(k*c)   */
}
