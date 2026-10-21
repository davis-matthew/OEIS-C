/*
 * A062283  Square array read by descending antidiagonals: T(n,k) =
 *          floor(n^k/k^n).
 * https://oeis.org/A062283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(n'^k' / k'^n') where (n',k') are the coordinates
 *            read by descending antidiagonals of the table T(n,k)=⌊n^k/k^n⌋
 * Range:     u8 n <= 93, u16 n <= 174, u32 n <= 382, u64 n <= 866,
 *            u128 n <= 2088, big any n
 * Helpers:   coords_from_index, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_from_str.h"
#include "../utils/coords_from_index.h"

#define BIG_BASE 1000000000U   /* 1e9 */

typedef struct {
    uint32_t d[80];   /* little‑endian digits, base 1e9, max ~ 720 digits */
    size_t len;       /* number of used digits, len==0 means zero */
} BigInt;

static void big_set_zero(BigInt *a) { a->len = 0; }

static void big_set_uint(BigInt *a, uint32_t v) {
    if (v == 0) { a->len = 0; return; }
    a->len = 0;
    while (v) { a->d[a->len++] = v % BIG_BASE; v /= BIG_BASE; }
}

static void big_mul_small(BigInt *a, uint32_t m) {
    if (m == 0 || a->len == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BIG_BASE);
        carry   = prod / BIG_BASE;
    }
    while (carry) {
        a->d[a->len++] = (uint32_t)(carry % BIG_BASE);
        carry /= BIG_BASE;
    }
}

static void big_div_small_inplace(BigInt *a, uint32_t d) {
    if (a->len == 0) return;
    uint64_t rem = 0;
    for (size_t i = a->len; i-- > 0; ) {
        uint64_t cur = rem * BIG_BASE + a->d[i];
        a->d[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    while (a->len && a->d[a->len - 1] == 0) --a->len;
}

/* Convert BigInt to decimal string.  Caller must provide a buffer
   large enough (here 512 bytes are enough for all required sizes). */
static void big_to_str(const BigInt *a, char *buf) {
    if (a->len == 0) {
        buf[0] = '0'; buf[1] = '\0'; return;
    }
    size_t i = a->len - 1;
    int pos = 0;
    /* first (most significant) digit without leading zeros */
    pos += sprintf(buf + pos, "%u", a->d[i]);
    /* remaining digits padded to 9 digits */
    for (i = i; i > 0; --i) {
        pos += sprintf(buf + pos, "%09u", a->d[i - 1]);
    }
    buf[pos] = '\0';
}

seq_t a(idx_t n) {
    idx_t np, kp;
    coords_from_index(n, &np, &kp);

    BigInt num;
    big_set_uint(&num, 1);

    /* numerator = np^kp */
    for (idx_t i = 0; i < kp; ++i)
        big_mul_small(&num, (uint32_t)np);

    /* floor division by kp repeated np times => floor(num / kp^np) */
    for (idx_t i = 0; i < np; ++i)
        big_div_small_inplace(&num, (uint32_t)kp);

    /* convert result to seq_t via string conversion */
    char buf[512];
    big_to_str(&num, buf);
    return seq_from_str(buf);
}
