/*
 * A010485  Decimal expansion of square root of 30.
 * https://oeis.org/A010485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 8974, u16 n <= 8974, u32 n <= 8974, u64 n <= 8974,
 *            u128 n <= 8974, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A010485:  Decimal expansion of √30
    a(n) = floor(10^n * √30) mod 10
*/

#include <stdlib.h>
#include <stdint.h>

/*------------------------------------------------------------*/
/* Big integer helpers – base 10 digits, little endian      */
/*------------------------------------------------------------*/
typedef struct {
    uint8_t *d;   /* array of digits (little endian) */
    size_t   len; /* number of digits used          */
    size_t   cap; /* allocated capacity             */
} bi_t;

static void bi_init(bi_t *b)
{
    b->d = NULL; b->len = 0; b->cap = 0;
}

static void bi_free(bi_t *b)
{
    free(b->d); b->d = NULL; b->len = b->cap = 0;
}

/* ensure capacity */
static void bi_need(bi_t *b, size_t need)
{
    if (b->cap < need) {
        size_t newcap = b->cap==0? 1 : b->cap;
        while (newcap < need) newcap <<= 1;
        b->d = (uint8_t*)realloc(b->d, newcap);
        b->cap = newcap;
    }
}

/* copy b2 into b1  (b1 may reuse its memory) */
static void bi_copy(bi_t *b1, const bi_t *b2)
{
    bi_need(b1, b2->len);
    for (size_t i = 0; i < b2->len; ++i) b1->d[i] = b2->d[i];
    b1->len = b2->len;
}

/* add small value (0-9) to bi (in place) */
static void bi_add_small(bi_t *b, uint8_t add)
{
    uint16_t carry = add;
    size_t i = 0;
    for (; i < b->len; ++i) {
        uint16_t s = b->d[i] + carry;
        b->d[i] = s % 10;
        carry   = s / 10;
        if (!carry) break;
    }
    if (carry) {
        bi_need(b, b->len+1);
        b->d[b->len++] = carry;
    }
}

/* multiply bi by small value (0-255)  (in place) */
static void bi_mul_small(bi_t *b, uint8_t mul)
{
    if (mul == 0) { b->len = 0; return; }
    uint16_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint16_t p = b->d[i]*mul + carry;
        b->d[i] = p % 10;
        carry   = p / 10;
    }
    while (carry) {
        bi_need(b, b->len+1);
        b->d[b->len++] = carry % 10;
        carry /= 10;
    }
}

/* subtract b2 from b1  (assumes b1>=b2)  (in place) */
static void bi_sub(bi_t *b1, const bi_t *b2)
{
    uint16_t borrow = 0;
    size_t i;
    for (i = 0; i < b1->len; ++i) {
        int sub = b1->d[i] - borrow - (i < b2->len ? b2->d[i] : 0);
        if (sub < 0) { sub += 10; borrow = 1; }
        else          borrow = 0;
        b1->d[i] = sub;
    }
    /* trim leading zeros */
    while (b1->len && b1->d[b1->len-1]==0) --b1->len;
}

/* compare two big integers: return 1 if a>b, -1 if a<b, 0 if equal */
static int bi_cmp(const bi_t *a, const bi_t *b)
{
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (size_t i = a->len; i > 0; --i) {
        uint8_t da = a->d[i-1];
        uint8_t db = b->d[i-1];
        if (da > db) return 1;
        if (da < db) return -1;
    }
    return 0;
}

/* wipe bi (free memory, reset) */
static void bi_clear(bi_t *b) { bi_free(b); bi_init(b); }

/*------------------------------------------------------------*/
/*  Main sequence function                                    */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* static cache of computed digits */
    static seq_t *digits = NULL;
    static size_t digits_cap = 0, digits_len = 0;

    /* big integer state */
    static bi_t P, R, T, tmp, prod;
    static int init = 0;

    if (!init) {                     /* initialise on first call   */
        init = 1;
        bi_init(&P); bi_init(&R); bi_init(&T);
        bi_init(&tmp); bi_init(&prod);

        /* integer part 5 */
        bi_need(&P, 1); P.d[0] = 5; P.len = 1;
        bi_need(&R, 1); R.d[0] = 5; R.len = 1;          /* 30-25 = 5 */

        digits_cap = 16;
        digits = (seq_t*)malloc(digits_cap*sizeof(seq_t));
        digits[0] = 5; digits_len = 1;
    }

    /* enlarge digit cache if necessary */
    if ((idx_t)n > digits_len) {
        size_t target = (size_t)n;
        if (target > digits_cap) {
            size_t newcap = digits_cap;
            while (newcap < target) newcap <<= 1;
            digits = (seq_t*)realloc(digits, newcap*sizeof(seq_t));
            digits_cap = newcap;
        }

        for (size_t i = digits_len; i < target; ++i) {
            /* R = R * 100  (bring down next “00”) */
            bi_mul_small(&R,100);

            /* T = P * 20  (20*current result) */
            bi_copy(&T,&P);
            bi_mul_small(&T,20);

            seq_t d;
            for (d = 9; ; --d) {
                /* tmp = T + d  */
                bi_copy(&tmp,&T);
                bi_add_small(&tmp,(uint8_t)d);

                /* prod = tmp * d  */
                bi_copy(&prod,&tmp);
                bi_mul_small(&prod,(uint8_t)d);

                /* check if prod <= R */
                if (bi_cmp(&prod,&R) <= 0) break;
            }

            /* R = R - prod */
            bi_sub(&R,&prod);

            /* P = P*10 + d   (append digit) */
            bi_mul_small(&P,10);
            bi_add_small(&P,(uint8_t)d);

            digits[i] = (seq_t)d;
        }
        digits_len = target;
    }
    return digits[n-1];
}
