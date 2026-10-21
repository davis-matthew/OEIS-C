/*
 * A094989  a(n) = floor(11^n/6^n).
 * https://oeis.org/A094989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(11^n / 6^n)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 73, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/*
 * Big integer used only when seq_t is 8 bytes or smaller.
 * Base 1 000 000 000 (fits in 32 bits).
 */
typedef struct {
    uint32_t *d;   /* least‑significant chunk first */
    size_t   len;  /* number of used chunks */
} BigInt;

/* ----------------- big‑int helpers -------------------------------- */

static void bi_init(BigInt *b)
{
    b->d   = NULL;
    b->len = 0;
}

static void bi_set_uint(BigInt *b, uint32_t v)
{
    b->d   = (uint32_t*)malloc(sizeof(uint32_t));
    if (!b->d) exit(1);        /* out of memory */
    b->d[0] = v;
    b->len  = 1;
}

static void bi_free(BigInt *b)
{
    free(b->d);
    b->d   = NULL;
    b->len = 0;
}

/* multiply by a small integer (m <= 11) */
static void bi_mul_small(BigInt *b, uint32_t m)
{
    if (m == 0) {
        bi_free(b);
        bi_set_uint(b, 0);
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)(prod % 1000000000U);
        carry   = prod / 1000000000U;
    }
    while (carry) {
        b->d = (uint32_t*)realloc(b->d, sizeof(uint32_t) * (b->len + 1));
        if (!b->d) exit(1);
        b->d[b->len] = (uint32_t)(carry % 1000000000U);
        carry        /= 1000000000U;
        ++b->len;
    }
}

/* divide by a small integer (d == 2 or 3) – truncating */
static void bi_div_small(BigInt *b, uint32_t d)
{
    if (d == 1 || b->len == 0) return;

    uint64_t rem = 0;
    for (int i = (int)b->len - 1; i >= 0; --i) {
        uint64_t cur = rem * 1000000000U + b->d[i];
        b->d[i] = (uint32_t)(cur / d);
        rem     = cur % d;
    }
    /* trim leading zero chunks */
    while (b->len > 1 && b->d[b->len - 1] == 0) --b->len;

    /* keep at least one chunk for zero */
    if (b->len == 0) {
        b->d   = (uint32_t*)malloc(sizeof(uint32_t));
        if (!b->d) exit(1);
        b->d[0] = 0;
        b->len  = 1;
    }
}

/* convert a big‑int that fits into 64 bits to uint64_t */
static uint64_t bi_to_uint64(const BigInt *b)
{
    unsigned __int128 val = 0;
    for (int i = (int)b->len - 1; i >= 0; --i)
        val = val * 1000000000U + b->d[i];
    return (uint64_t)val;
}

/* ----------------- main sequence function ------------------------- */

/* If seq_t is a plain unsigned integer, the calculation uses a big integer
 * once seq_t is 8 bytes or smaller.  For larger seq_t types (128‑bit, bigint)
 * the compiler's own arithmetic is sufficient.                     */
seq_t a(idx_t n)
{
    /* --- big‑int path for 8‑byte or smaller seq_t ------------------- */
    if (sizeof(seq_t) <= 8) {
        BigInt num;
        bi_init(&num);
        bi_set_uint(&num, 1);                       /* num = 1 */

        /* num = 11^n */
        for (idx_t i = 0; i < n; ++i)
            bi_mul_small(&num, 11U);

        /* divide by 2^n and 3^n (i.e. by 6^n) */
        for (idx_t i = 0; i < n; ++i)
            bi_div_small(&num, 2U);
        for (idx_t i = 0; i < n; ++i)
            bi_div_small(&num, 3U);

        uint64_t r = bi_to_uint64(&num);
        bi_free(&num);
        return (seq_t)r;
    }

    /* --- direct arithmetic for larger seq_t (≥128‑bit or bigint) ----- */
    /* Large integer arithmetic is assumed to be supported; use pow
     * by repeated squaring to keep the number of multiplications small.   */
    /* Compute 11^n */
    seq_t pow11 = 1;
    seq_t base = 11;
    idx_t exp = n;
    while (exp) {
        if (exp & 1U) pow11 *= base;
        base *= base;
        exp >>= 1U;
    }

    /* Compute 6^n using similar method */
    seq_t pow6 = 1;
    base = 6;
    exp = n;
    while (exp) {
        if (exp & 1U) pow6 *= base;
        base *= base;
        exp >>= 1U;
    }

    return pow11 / pow6;
}
