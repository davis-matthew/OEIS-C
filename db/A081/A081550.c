/*
 * A081550  Decimal expansion of Sum_(1/(2^q-1)) with the summation extending
 *          over all pairs of integers gcd(p,q) = 1, 0 < p/q < Pi.
 * https://oeis.org/A081550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = decimal digit n of the series Sum_{k>=1}
 *            (1/2)^{floor(k/Pi)} (offset 1) Equivalent to S = Sum_{m>=0} c_m
 *            * 2^{-m} where c_m = floor((m+1)*Pi) - floor(m*Pi) (c_m is 3 or
 *            4). a(n) are the decimal digits of S
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* Simple big‑integer with base 2^32, little endian */
typedef struct {
    uint32_t *v;   /* vector of limbs */
    size_t len;    /* number of limbs used */
} BigInt;

static void bigint_init(BigInt *b, size_t cap) {
    b->v = (uint32_t*)malloc(cap * sizeof(uint32_t));
    b->len = 0;
}

static void bigint_free(BigInt *b) {
    free(b->v);
}

static void bigint_reserve(BigInt *b, size_t need) {
    if (need <= (b->len * 2)) return;
    size_t newcap = need;
    b->v = (uint32_t*)realloc(b->v, newcap * sizeof(uint32_t));
}

/* remove trailing zero limbs */
static void bigint_norm(BigInt *b) {
    while (b->len && b->v[b->len-1]==0) b->len--;
}

/* add small integer val (≤4) shifted by shift bits */
static void bigint_add_shifted(BigInt *b, uint32_t val, int shift) {
    int blim = shift >> 5;          /* limb index */
    int boff = shift & 31;          /* bit offset */
    /* ensure enough limbs */
    if (blim + 2 > b->len) {
        bigint_reserve(b, blim + 2);
        for (size_t i=b->len;i<=blim+1;i++) b->v[i]=0;
        b->len = blim+2;
    }
    uint64_t add_low  = ((uint64_t)val) << boff;
    uint64_t sum      = (uint64_t)b->v[blim] + (add_low & 0xFFFFFFFF);
    b->v[blim] = (uint32_t)sum;
    uint64_t carry = (sum >> 32) + (add_low >> 32);

    size_t i = blim + 1;
    while (carry) {
        uint64_t s = (uint64_t)b->v[i] + carry;
        b->v[i] = (uint32_t)s;
        carry = s >> 32;
        ++i;
    }
    if (i > b->len) b->len = i;
    bigint_norm(b);
}

/* multiply by small integer 10 */
static void bigint_mul10(BigInt *b) {
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->v[i] * 10 + carry;
        b->v[i] = (uint32_t)prod;
        carry = prod >> 32;
    }
    if (carry) {
        if (b->len == 0) {
            b->v[0] = (uint32_t)carry;
            b->len = 1;
        } else {
            b->v[b->len++] = (uint32_t)carry;
        }
    }
}

/* extract digit = floor(R/2^M) where 0 <= R < 2^(M+4)
   and replace R by R mod 2^M (low M bits) */
static uint32_t bigint_get_digit_and_reduce(BigInt *b, int M) {
    int limb = M >> 5;
    int off  = M & 31;
    uint32_t digit = 0;

    if (off == 0) {
        if (limb < b->len) digit = b->v[limb] & 0xF;   /* higher bits discarded later */
        /* keep only low bits < M */
        for (size_t i = limb; i < b->len; ++i) b->v[i] = 0;
    } else {
        uint64_t part = 0;
        if (limb < b->len) part |= (uint64_t)b->v[limb] >> off;
        if (limb + 1 < b->len)
            part |= (uint64_t)b->v[limb + 1] << (32 - off);
        digit = part & 0xF;
        /* mask low bits */
        if (b->len > limb) b->v[limb] &= ((1U << off) - 1);
        for (size_t i = limb + 1; i < b->len; ++i) b->v[i] = 0;
    }
    bigint_norm(b);
    return digit;
}

/* retrieve integer part = floor(N/2^M) */
static uint32_t bigint_int_part(const BigInt *b, int M) {
    int limb = M >> 5;
    int off = M & 31;
    uint32_t res = 0;
    if (off == 0) {
        if (limb < b->len) res = b->v[limb];
    } else {
        if (limb < b->len) res = b->v[limb] >> off;
        if (limb + 1 < b->len) res |= b->v[limb + 1] << (32 - off);
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 1) {
        /* For n=1 we still need the integer part; compute with minimal bits */
    }
    /* estimate required bits: M > n*log2(10)+5 */
    int M = (int)(n * 3.322) + 10;
    if (M < 1) M = 1;

    /* build numerator N */
    BigInt N;
    bigint_init(&N, (M + 6)/32 + 1);
    /* precompute floor(m*Pi) progressively to avoid many multiplies */
    double PI = acos(-1.0);
    double val = 0.0;          /* current floor(m*Pi) as double */
    int prev_floor = 0;
    for (int m = 0; m < M; ++m) {
        double v1 = (m + 1) * PI;
        int f1 = (int)floor(v1 + 1e-12);
        int c = f1 - prev_floor;   /* c_m = floor((m+1)*Pi) - floor(m*Pi) */
        prev_floor = f1;
        int shift = M - m;
        bigint_add_shifted(&N, (uint32_t)c, shift);
    }

    /* integer part */
    uint32_t int_part = bigint_int_part(&N, M);

    if (n == 1)
        return (seq_t)int_part;

    /* remainder R = N mod 2^M */
    BigInt R;
    bigint_init(&R, (M + 6)/32 + 1);
    size_t low_limbs = (M + 31) >> 5;
    R.len = low_limbs;
    for (size_t i = 0; i < low_limbs; ++i) {
        R.v[i] = (i < N.len) ? N.v[i] : 0;
    }
    /* mask last limb */
    int last_bits = M & 31;
    if (last_bits) {
        uint32_t mask = (1U << last_bits) - 1;
        R.v[low_limbs - 1] &= mask;
    }
    bigint_norm(&R);

    /* iterate to the requested digit */
    for (int idx = 2; idx <= n; ++idx) {    /* idx=2 corresponds to second digit after decimal */
        bigint_mul10(&R);
        uint32_t digit = bigint_get_digit_and_reduce(&R, M);
        if (idx == n)
            return (seq_t)digit;
    }

    /* should never reach here */
    return (seq_t)0;
}
