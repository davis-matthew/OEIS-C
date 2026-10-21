/*
 * A077774  Number of integers between n^2 and (n+1)^2 that are the sum of two
 *          coprime squares of opposite parity; multiple representations are
 *          counted once.
 * https://oeis.org/A077774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithmic computation of OEIS A077774:
   Number of integers between n^2 and (n+1)^2 that are the sum of two coprime
   squares of opposite parity; each such integer is counted once. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Helper functions ---------- */
static inline idx_t isqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi)
    {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static inline idx_t ugcd(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline int get_bit(const uint8_t *bits, idx_t k)
{
    return (bits[k >> 3] >> (k & 7)) & 1;
}

static inline void set_bit(uint8_t *bits, idx_t k)
{
    bits[k >> 3] |= (uint8_t)(1 << (k & 7));
}

/* ---------- Static caching structures ---------- */
static uint8_t *bits = NULL;                /* bit array for sums */
static size_t bits_size_bytes = 0;          /* allocated bytes */
static idx_t bits_size = 0;                 /* maximum index stored (inclusive) */
static idx_t *b_limit_processed = NULL;     /* for each a, processed max b */
static size_t b_limit_len = 0;              /* size of b_limit_processed */

/* Ensure that sums up to kmax are precomputed. */
static void ensure_precomputation(idx_t n)
{
    /* required maximum k */
    idx_t req_k = (idx_t)(n + 1) * (n + 1) - 1;
    if (req_k <= bits_size)
        return; /* already enough */

    /* resize bit array */
    size_t new_bits_bytes = (req_k >> 3) + 1;
    uint8_t *new_bits = (uint8_t *)realloc(bits, new_bits_bytes);
    if (!new_bits)
        abort();
    if (new_bits_bytes > bits_size_bytes)
        memset(new_bits + bits_size_bytes, 0,
               new_bits_bytes - bits_size_bytes);
    bits = new_bits;
    bits_size_bytes = new_bits_bytes;
    bits_size = req_k; /* last index that can be stored */

    /* resize b_limit_processed array */
    idx_t new_limit_a = isqrt(req_k);
    if ((size_t)new_limit_a + 1 > b_limit_len)
    {
        size_t new_len = (size_t)new_limit_a + 1;
        idx_t *new_b_limit = (idx_t *)realloc(b_limit_processed,
                                               new_len * sizeof(idx_t));
        if (!new_b_limit)
            abort();
        if (new_len > b_limit_len)
            memset(new_b_limit + b_limit_len, 0,
                   (new_len - b_limit_len) * sizeof(idx_t));
        b_limit_processed = new_b_limit;
        b_limit_len = new_len;
    }

    /* process new pairs */
    for (idx_t a = 1; a <= new_limit_a; ++a)
    {
        idx_t a2 = a * a;
        if (a2 > req_k)
            break; /* no b possible */
        idx_t new_limit_b = isqrt(req_k - a2);
        idx_t prev_b = b_limit_processed[a];
        if (new_limit_b <= prev_b)
            continue; /* nothing new for this a */
        for (idx_t b = prev_b + 1; b <= new_limit_b; ++b)
        {
            if (((a & 1) == (b & 1))) /* not opposite parity */
                continue;
            if (ugcd(a, b) != 1) /* not coprime */
                continue;
            idx_t s = a2 + b * b; /* sum */
            if (s <= req_k)
                set_bit(bits, s);
        }
        b_limit_processed[a] = new_limit_b;
    }
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    ensure_precomputation(n);

    idx_t lo = n * n;
    idx_t hi = (idx_t)(n + 1) * (n + 1) - 1;
    seq_t cnt = 0;
    for (idx_t k = lo + 1; k <= hi; ++k)
    {
        if (get_bit(bits, k))
            ++cnt;
    }
    return cnt;
}
