/*
 * A105564  Number of blocks of exactly 4 Fibonacci numbers having equal
 *          length <= n.
 * https://oeis.org/A105564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of decimal digit lengths L ≤ n for which the
 *            consecutive Fibonacci numbers of length L form a run of exactly
 *            4 numbers
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   digits_limb
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/digits_limb.h"

/* --------- Big integer with base 1e9 (enough for 10000 digits) --------- */
#define MAX_DIGITS 10000                     /* maximum decimal digits required   */
#define BASE 1000000000U                     /* 1e9 base */
#define LIMBS ((MAX_DIGITS + 8) / 9 + 5)     /* safe number of limbs (≈1115)      */

typedef struct {
    uint32_t d[LIMBS];
    size_t   len;
} BigUint;

/* initialise a BigUint to a small integer value */
static void init_small(BigUint *x, uint32_t v)
{
    x->len = 1;
    x->d[0] = v;
}

/* add two BigUints: res = a + b */
static void add_big(const BigUint *a, const BigUint *b, BigUint *res)
{
    size_t  m = a->len > b->len ? a->len : b->len;
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < m || carry; ++i) {
        uint64_t sum = carry;
        if (i < a->len) sum += a->d[i];
        if (i < b->len) sum += b->d[i];
        res->d[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
    }
    res->len = i;
    if (res->len == 0) {          /* never happens, but keep invariant */
        res->len = 1;
        res->d[0] = 0;
    }
}

/* number of decimal digits in a BigUint */
static int big_uint_digits(const BigUint *x)
{
    return (int)((x->len - 1) * 9 + digits_limb(x->d[x->len - 1]));
}

/* ------------------------- sequence function -------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* initialise first two Fibonacci numbers: 1, 1 */
    BigUint f1, f2, f3;
    init_small(&f1, 1);
    init_small(&f2, 1);

    BigUint *pa = &f1, *pb = &f2, *pc = &f3;

    idx_t cur_len = big_uint_digits(pb);   /* 1 */
    idx_t run_len = 2;                     /* the two Fibs of length 1 */
    seq_t  ans     = 0;

    for (;;) {
        /* compute next Fibonacci number */
        add_big(pa, pb, pc);

        /* rotate the pointers: pa <- pb, pb <- pc, pc <- old pa */
        BigUint *tmp = pa; pa = pb; pb = pc; pc = tmp;

        idx_t d = big_uint_digits(pb);     /* digit length of new Fibo */

        if (d > n) {                      /* finished all lengths ≤ n */
            if (run_len == 4)              /* final run before break */
                ans += 1;
            break;
        }

        if (d != cur_len) {               /* run ended, start new */
            if (run_len == 4)              /* count previous run if size 4 */
                ans += 1;
            cur_len = d;
            run_len = 1;                   /* only the new Fibo so far */
        } else {
            run_len++;                     /* still same run length */
        }
    }

    return ans;
}
