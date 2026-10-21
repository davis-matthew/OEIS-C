/*
 * A088646  a(0) = 1 and then alternately prime and composite numbers such
 *          that the successive difference is (distinct) composite and prime
 *          respectively.
 * https://oeis.org/A088646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088646: a(0)=1; thereafter terms alternate prime/composite.
   The successive difference is a distinct composite when the new term
   is prime, and a distinct prime when the new term is composite. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  64‑bit Miller–Rabin primality test (deterministic)                  */
/* ------------------------------------------------------------------ */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (__uint128_t)a * b % mod;
}

static inline uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t mod)
{
    __uint128_t res = 1, base = a % mod;
    while (e) {
        if (e & 1) res = res * base % mod;
        base = base * base % mod;
        e >>= 1;
    }
    return (uint64_t)res;
}

static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Simple dynamic bit‑set for used differences                        */
/* ------------------------------------------------------------------ */
typedef struct {
    uint8_t *bits;      /* one bit per integer value */
    uint64_t bitsize;   /* number of bits currently supported  */
} bitset_t;

static void bitset_init(bitset_t *bs, uint64_t init)
{
    bs->bitsize = init;
    if (bs->bitsize == 0) {
        bs->bits = NULL;
    } else {
        uint64_t bytes = (bs->bitsize + 7) >> 3;
        bs->bits = (uint8_t *)calloc(bytes, 1);
    }
}

static void bitset_resize(bitset_t *bs, uint64_t need)
{
    if (need < bs->bitsize) return;
    uint64_t newsize = bs->bitsize ? bs->bitsize * 2 : 64;
    while (newsize <= need) newsize <<= 1;
    uint64_t newbytes = (newsize + 7) >> 3;
    uint64_t oldbytes = (bs->bitsize + 7) >> 3;
    bs->bits = (uint8_t *)realloc(bs->bits, newbytes);
    if (bs->bits) memset(bs->bits + oldbytes, 0, newbytes - oldbytes);
    bs->bitsize = newsize;
}

static inline bool bitset_test(const bitset_t *bs, uint64_t val)
{
    if (val >= bs->bitsize) return false;
    return (bs->bits[val >> 3] >> (val & 7)) & 1;
}

static void bitset_set(bitset_t *bs, uint64_t val)
{
    if (val >= bs->bitsize) bitset_resize(bs, val);
    bs->bits[val >> 3] |= 1 << (val & 7);
}

/* ------------------------------------------------------------------ */
/*  Sequence storage and helpers                                       */
/* ------------------------------------------------------------------ */
static seq_t *seq = NULL;      /* sequence values */
static idx_t seq_len = 0;      /* number of values stored */
static idx_t seq_cap = 0;      /* allocated capacity */

static void seq_ensure(idx_t need)
{
    if (need < seq_cap) return;
    idx_t newcap = seq_cap ? seq_cap << 1 : 16;
    while (newcap <= need) newcap <<= 1;
    seq = (seq_t *)realloc(seq, sizeof(seq_t) * newcap);
    seq_cap = newcap;
}

static bitset_t diff_prime = {0,0};
static bitset_t diff_comp  = {0,0};

/* ------------------------------------------------------------------ */
/*  Main function                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (seq_len == 0) {
        seq_ensure(1);
        seq[0] = 1;
        seq_len = 1;
        bitset_init(&diff_prime, 64);
        bitset_init(&diff_comp, 64);
    }

    /* Grow cache up to the required index */
    while (seq_len <= n) {
        uint64_t prev = (uint64_t)seq[seq_len - 1];
        uint64_t cand;
        uint64_t diff;

        if (seq_len % 2 == 1) {               /* need a prime */
            cand = prev + 1;
            for (;;) {
                if (!is_prime64(cand)) { cand++; continue; }
                diff = cand - prev;
                if (diff <= 1) { cand++; continue; }
                if (is_prime64(diff)) { cand++; continue; } /* diff must be composite */
                if (bitset_test(&diff_comp, diff)) { cand++; continue; }
                break;                           /* accepted */
            }
            seq_ensure(seq_len);
            seq[seq_len++] = (seq_t)cand;
            bitset_set(&diff_comp, diff);
        } else {                               /* need a composite */
            cand = prev + 1;
            for (;;) {
                if (is_prime64(cand)) { cand++; continue; }
                diff = cand - prev;
                if (diff <= 1) { cand++; continue; }
                if (!is_prime64(diff)) { cand++; continue; } /* diff must be prime */
                if (bitset_test(&diff_prime, diff)) { cand++; continue; }
                break;                           /* accepted */
            }
            seq_ensure(seq_len);
            seq[seq_len++] = (seq_t)cand;
            bitset_set(&diff_prime, diff);
        }
    }

    return seq[n];
}
