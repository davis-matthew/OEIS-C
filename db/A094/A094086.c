/*
 * A094086  Babylonian reciprocals - factors needed to turn an ugly number
 *          into a power of sixty.
 * https://oeis.org/A094086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 60^k / u_n where u_n is the n‑th ugly number (u_n = 2^e2
 *            · 3^e3 · 5^e5) and k = max( ceil(e2/2), e3, e5 )
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer power for arbitrary seq_t types. */
static seq_t pow_seq_t_local(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Cache of ugly numbers and their prime exponents. */
static uint64_t *ugl_val   = NULL;
static unsigned *ugl_e2    = NULL;
static unsigned *ugl_e3    = NULL;
static unsigned *ugl_e5    = NULL;
static size_t   ugl_capacity = 0;
static size_t   ugl_size     = 0;
static size_t   i2 = 0, i3 = 0, i5 = 0;
static int ugl_initialized = 0;

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (ugl_capacity >= need)
        return;
    size_t newcap = ugl_capacity ? ugl_capacity * 2 : 128;
    while (newcap < need)
        newcap <<= 1;
    ugl_val   = (uint64_t *)realloc(ugl_val,   newcap * sizeof(uint64_t));
    ugl_e2    = (unsigned *)realloc(ugl_e2,    newcap * sizeof(unsigned));
    ugl_e3    = (unsigned *)realloc(ugl_e3,    newcap * sizeof(unsigned));
    ugl_e5    = (unsigned *)realloc(ugl_e5,    newcap * sizeof(unsigned));
    ugl_capacity = newcap;
}

/* Generate ugly numbers up to (and including) index 'idx' (0‑based). */
static void generate_upl_to(idx_t idx)
{
    if (!ugl_initialized)
    {
        ensure_cache(128);
        ugl_val[0] = 1ULL;
        ugl_e2[0]  = 0;
        ugl_e3[0]  = 0;
        ugl_e5[0]  = 0;
        ugl_size   = 1;
        i2 = i3 = i5 = 0;
        ugl_initialized = 1;
    }

    while (ugl_size <= (size_t)idx)
    {
        uint64_t v2 = ugl_val[i2] * 2ULL;
        uint64_t v3 = ugl_val[i3] * 3ULL;
        uint64_t v5 = ugl_val[i5] * 5ULL;

        uint64_t next = v2;
        if (v3 < next) next = v3;
        if (v5 < next) next = v5;

        unsigned e2, e3, e5;
        if (next == v2)
        { e2 = ugl_e2[i2] + 1; e3 = ugl_e3[i2]; e5 = ugl_e5[i2]; }
        else if (next == v3)
        { e2 = ugl_e2[i3]; e3 = ugl_e3[i3] + 1; e5 = ugl_e5[i3]; }
        else
        { e2 = ugl_e2[i5]; e3 = ugl_e3[i5]; e5 = ugl_e5[i5] + 1; }

        ensure_cache(ugl_size + 1);
        ugl_val[ugl_size] = next;
        ugl_e2[ugl_size]  = e2;
        ugl_e3[ugl_size]  = e3;
        ugl_e5[ugl_size]  = e5;
        ++ugl_size;

        if (v2 == next) ++i2;
        if (v3 == next) ++i3;
        if (v5 == next) ++i5;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined below offset */
    generate_upl_to(n - 1);             /* get the n‑th ugly number */

    unsigned e2 = ugl_e2[n - 1];
    unsigned e3 = ugl_e3[n - 1];
    unsigned e5 = ugl_e5[n - 1];

    idx_t k = (idx_t)((e2 + 1) / 2);    /* ceil(e2/2) */
    if (e3 > (unsigned)k) k = e3;
    if (e5 > (unsigned)k) k = e5;
    if (k == 0) k = 1;                 /* for the case e2=e3=e5=0 */

    unsigned pow2 = 2 * k - e2;
    unsigned pow3 = k - e3;
    unsigned pow5 = k - e5;

    seq_t res = (seq_t)1;
    if (pow2 > 0)
        res = res * pow_seq_t_local((seq_t)2, pow2);
    if (pow3 > 0)
        res = res * pow_seq_t_local((seq_t)3, pow3);
    if (pow5 > 0)
        res = res * pow_seq_t_local((seq_t)5, pow5);

    return res;
}
