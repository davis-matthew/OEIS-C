/*
 * A095254  a(n) = floor(concatenation(1,2,3,...,n-1,n)^(1/n)).
 * https://oeis.org/A095254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor(concat(1,2,…,n)^(1/n))
 * Range:     u8 n <= 184, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <string.h>

/* use an unbounded big integer limited only by a static buffer  */
#define BASE 1000000000U          /* 1e9 */
#define MAX_WORDS 5000            /* enough for 40 000 decimal digits */

/* big integer: little‑endian base 10^9 array */
struct BigInt {
    int len;                     /* number of used words (>=1) */
    uint32_t d[MAX_WORDS];
};

/* initialise to zero */
static void bi_zero(struct BigInt *x)
{
    x->len = 1;
    x->d[0] = 0;
}

/* initialise to one */
static void bi_one(struct BigInt *x)
{
    x->len = 1;
    x->d[0] = 1;
}

/* trim leading zero words */
static void bi_trim(struct BigInt *x)
{
    while (x->len > 1 && x->d[x->len - 1] == 0)
        x->len--;
}

/* multiply *x by small integer m (m <= 100000) */
static void bi_mul_small(struct BigInt *x, uint32_t m)
{
    if (m == 0) {
        bi_zero(x);
        return;
    }
    uint64_t carry = 0;
    for (int i = 0; i < x->len; i++) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(prod % BASE);
        carry   = prod / BASE;
    }
    while (carry) {
        x->d[x->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* add small integer v (v <= 100000) to *x */
static void bi_add_small(struct BigInt *x, uint32_t v)
{
    uint64_t carry = v;
    int i = 0;
    while (carry && i < x->len) {
        uint64_t sum = (uint64_t)x->d[i] + carry;
        x->d[i] = (uint32_t)(sum % BASE);
        carry   = sum / BASE;
        i++;
    }
    if (carry) {
        x->d[x->len++] = (uint32_t)carry;
    }
}

/* compare a and b: <0 if a<b, 0 if equal, >0 if a>b */
static int bi_cmp(const struct BigInt *a, const struct BigInt *b)
{
    if (a->len != b->len)
        return (a->len < b->len) ? -1 : 1;
    for (int i = a->len - 1; i >= 0; i--) {
        if (a->d[i] != b->d[i])
            return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* compute base^exp (base <= 100000), compare with C:
   return -1 if <, 0 if ==, +1 if > */
static int cmp_pow(const struct BigInt *C, uint32_t base, idx_t exp)
{
    struct BigInt res;
    bi_one(&res);
    for (idx_t i = 0; i < exp; ++i) {
        bi_mul_small(&res, base);
        int cmp = bi_cmp(&res, C);
        if (cmp > 0)
            return 1;           /* already greater */
    }
    return bi_cmp(&res, C);
}

/* list of powers of ten up to 10^5 */
static const uint32_t POW10[6] = {1, 10, 100, 1000, 10000, 100000};

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but return 0 */

    struct BigInt C;
    bi_zero(&C);

    /* for building a prefix of the leading digits */
    char prefix[20];
    int pref_len = 0;

    uint64_t total_digits = 0;

    /* build the concatenated number */
    for (idx_t k = 1; k <= n; ++k) {
        /* determine number of decimal digits of k */
        int d;
        if (k < 10ULL)          d = 1;
        else if (k < 100ULL)    d = 2;
        else if (k < 1000ULL)   d = 3;
        else if (k < 10000ULL)  d = 4;
        else                   d = 5;          /* k <= 10000 for our limits */

        bi_mul_small(&C, POW10[d]);   /* C *= 10^d */
        bi_add_small(&C, (uint32_t)k);/* C += k */

        /* accumulate prefix of first digits (up to 17) */
        char buf[6];
        int len = sprintf(buf, "%u", (unsigned)k);
        total_digits += len;
        if (pref_len < 17) {
            int need = 17 - pref_len;
            if (need > len) need = len;
            memcpy(prefix + pref_len, buf, need);
            pref_len += need;
        }
    }

    /* log10 of prefix value */
    long double prefix_val = strtold(prefix, NULL);
    long double log10_prefix = log10(prefix_val);
    long double log10C = log10_prefix + (long double)(total_digits - pref_len);

    /* approximate nth root */
    long double root_est_ld = pow(10.0L, log10C / (long double)n);
    uint32_t r_est = (uint32_t)(root_est_ld + 1e-9L);
    if (r_est == 0) r_est = 1;

    /* adjust upward while (r_est+1)^n <= C */
    while (1) {
        int cmp = cmp_pow(&C, r_est + 1, n);
        if (cmp <= 0)
            ++r_est;
        else
            break;
    }
    /* adjust downward while r_est^n > C */
    while (1) {
        int cmp = cmp_pow(&C, r_est, n);
        if (cmp > 0)
            --r_est;
        else
            break;
    }

    return (seq_t)r_est;
}
