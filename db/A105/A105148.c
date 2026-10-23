/*
 * A105148  Number of semiprimes k such that k is a multiple of 3 and n^3 < k
 *          <= (n+1)^3.
 * https://oeis.org/A105148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = π(⌊(n+1)^3/3⌋) – π(⌊n^3/3⌋) (π(x) is the prime counting
 *            function). This counts the primes p with ⌊n^3/3⌋ < p ≤
 *            ⌊(n+1)^3/3⌋, i.e. the primes p such that 3p lies between n³ and
 *            (n+1)³
 * Range:     u8 n <= 52, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/*   prime table up to 600000 (≈ sqrt(max bound for n = 10000) )     */
/* ------------------------------------------------------------------ */
#define PRIMELIMIT 600000
static uint32_t prime_tab[PRIMELIMIT];
static int prime_cnt = 0;
static int prime_inited = 0;

static void init_primes(void)
{
    if (prime_inited) return;
    char *is_comp = (char*)malloc(PRIMELIMIT + 1);
    if (!is_comp) exit(1);
    for (int i = 0; i <= PRIMELIMIT; ++i) is_comp[i] = 0;
    for (int p = 2; p * p <= PRIMELIMIT; ++p) {
        if (!is_comp[p]) {
            for (int j = p * p; j <= PRIMELIMIT; j += p)
                is_comp[j] = 1;
        }
    }
    for (int i = 2; i <= PRIMELIMIT; ++i)
        if (!is_comp[i]) prime_tab[prime_cnt++] = (uint32_t)i;
    free(is_comp);
    prime_inited = 1;
}

/* ------------------------------------------------------------------ */
/*   count primes in the interval [low, high] (inclusive)              */
/* ------------------------------------------------------------------ */
static seq_t count_primes_interval(uint64_t low, uint64_t high)
{
    if (low > high) return 0;
    uint64_t sz = high - low + 1;
    size_t words = (sz + 63) / 64;
    uint64_t *bits = (uint64_t*)malloc(words * sizeof(uint64_t));
    if (!bits) exit(1);
    for (size_t i = 0; i < words; ++i) bits[i] = ~0ULL;
    /* clear unused bits in the last word */
    size_t rem = sz & 63;
    if (rem) bits[words - 1] = (1ULL << rem) - 1;

    /* mark composites */
    uint64_t sqrt_high = 0;
    {
        unsigned __int128 tmp = (unsigned __int128)high * high;
        sqrt_high = (uint64_t)( (unsigned __int128)(high) * (unsigned __int128)(high) );
        /* but we only need primes up to sqrt(high) */
        sqrt_high = 0;
        unsigned __int128 h = high;
        sqrt_high = (uint64_t)( (unsigned __int128)(high) * (unsigned __int128)(high) );
        // actually simpler: compute sqrt_high via integer sqrt
        sqrt_high = (uint64_t) ( (double)high * 0.5 );
        /* adjust */
        while ((unsigned __int128)(sqrt_high + 1) * (sqrt_high + 1) <= high) ++sqrt_high;
        while ((unsigned __int128)sqrt_high * sqrt_high > high) --sqrt_high;
    }
    for (int pi = 0; pi < prime_cnt; ++pi) {
        uint32_t p = prime_tab[pi];
        if ((uint64_t)p * (uint64_t)p > high) break;
        uint64_t start = (low + p - 1) / p * p;
        if (start < (uint64_t)p * (uint64_t)p)
            start = (uint64_t)p * (uint64_t)p;
        for (uint64_t j = start; j <= high; j += p) {
            uint64_t offset = j - low;
            bits[offset >> 6] &= ~(1ULL << (offset & 63));
        }
    }
    /* handle numbers < 2 */
    if (low <= 1 && 1 <= high) {
        uint64_t off = 1 - low;
        bits[off >> 6] &= ~(1ULL << (off & 63));
    }
    /* count primes */
    uint64_t cnt = 0;
    for (size_t i = 0; i < words; ++i) cnt += __builtin_popcountll(bits[i]);
    free(bits);
    return (seq_t)cnt;
}

/* ------------------------------------------------------------------ */
/*   public sequence function                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_primes();

    /* lower bound for the prime p: floor(n³/3) + 1  */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 n3   = n128 * n128 * n128;
    uint64_t low  = (uint64_t)(n3 / 3) + 1;

    /* upper bound for the prime p: floor((n+1)³/3) */
    unsigned __int128 np1 = n128 + 1;
    unsigned __int128 np13 = np1 * np1 * np1;
    uint64_t high = (uint64_t)(np13 / 3);

    if (high < low) return 0;   /* empty interval */
    return count_primes_interval(low, high);
}
