/*
 * A086357  Fixed point if the nonzero-digit-product function is iterating
 *          starting from the n-th primorial A002110(n).
 * https://oeis.org/A086357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A051802(A002110(n)) where A002110(n) is the n-th
 *            primorial and A051802 applies the non‑zero digit product
 *            iteratively until a single digit is reached
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* --- Big integer (base 1 000 000 000) --------------------------------- */
typedef struct {
    uint32_t *d;   /* little‑endian limbs, each < BASE */
    size_t    len; /* number of used limbs (len==0 means 0) */
    size_t    cap; /* allocated capacity in limbs            */
} Bi;

#define BASE 1000000000U

static void bi_init(Bi *x)
{
    x->cap = 4;
    x->len = 0;
    x->d   = (uint32_t *)malloc(x->cap * sizeof(uint32_t));
}

static void bi_free(Bi *x)
{
    free(x->d);
}

static void bi_set_one(Bi *x)
{
    x->len = 1;
    x->d[0] = 1;
}

static void bi_mul_small(Bi *x, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t cur = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(cur % BASE);
        carry   = cur / BASE;
    }
    while (carry) {
        if (x->len == x->cap) {
            x->cap <<= 1;
            x->d = (uint32_t *)realloc(x->d, x->cap * sizeof(uint32_t));
        }
        x->d[x->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* Convert to decimal string (buf must be large enough) */
static void bi_to_str(const Bi *x, char *buf, size_t *len)
{
    if (x->len == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    int pos = 0;
    pos += snprintf(buf + pos, 12, "%u", x->d[x->len - 1]);
    for (int i = (int)x->len - 2; i >= 0; --i) {
        pos += snprintf(buf + pos, 10, "%09u", x->d[i]); /* pad 9 digits */
    }
    *len = (size_t)pos;
}

/* Return 1 if Big has a single decimal digit <10 */
static int bi_single_digit(const Bi *x)
{
    return x->len == 1 && x->d[0] < 10;
}

/* --- Prime generation (up to 200 000) --------------------------------- */
#define PRIME_LIMIT 200000
static uint32_t primes[PRIME_LIMIT / 2];
static int prime_cnt = 0;
static int primes_ready = 0;

static void init_primes(void)
{
    if (primes_ready) return;
    static unsigned char composite[PRIME_LIMIT + 1];
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if ((int64_t)i * i <= PRIME_LIMIT) {
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    composite[j] = 1;
            }
        }
    }
    primes_ready = 1;
}

/* --- Sequence a(n) --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* behaviour for n<1 is irrelevant */

    init_primes();            /* ensure primes are ready */

    /* (1) Compute primorial as big integer */
    Bi prim;
    bi_init(&prim);
    bi_set_one(&prim);
    for (idx_t i = 0; i < n; ++i)
        bi_mul_small(&prim, primes[i]);

    /* (2) Iterate digit product until single digit */
    while (!bi_single_digit(&prim)) {
        static char dec[70000];   /* ample space for ~40 000 digits */
        size_t len;
        bi_to_str(&prim, dec, &len);

        Bi prod;
        bi_init(&prod);
        bi_set_one(&prod);

        for (size_t j = 0; j < len; ++j) {
            char c = dec[j];
            if (c != '0')
                bi_mul_small(&prod, (uint32_t)(c - '0'));
        }

        bi_free(&prim);
        prim = prod;          /* take ownership of new big integer */
    }

    seq_t result = (seq_t)(prim.d[0] % 10);  /* single digit */
    bi_free(&prim);
    return result;
}
