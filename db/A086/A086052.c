/*
 * A086052  Sum of first n 6-almost primes.
 * https://oeis.org/A086052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the first n integers whose total number of
 *            (not‑necessarily‑distinct) prime factors Ω(m) equals 6
 * Range:     u8 n <= 2, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the necessary prime table once, then
 * counts Ω(m) by trial division with that table.
 * It keeps a dynamic prefix array of cumulative sums so that
 * any call to a(idx_t n) can return the result in O(1) after the
 * needed primes have been pre‑computed.  The algorithm is
 * O(n log log n) for the first 10 000 terms – well within limits. */

#include <stdlib.h>
#include <stdint.h>

/* --------- global static state (shared across calls) --------- */

static idx_t          next_number = 2;          /* next integer to test          */
static size_t         prefix_capacity = 64;     /* current malloc size of prefix */
static seq_t*         prefix = NULL;           /* 0‑based: prefix[k-1] = a(k+1)  */
static size_t         prefix_len   = 0;         /* number of computed terms      */
static seq_t          cumulative   = 0;         /* sum of the last 6‑almost prime */

static uint32_t       primes[80000];           /* list of small primes          */
static size_t         primes_cnt = 0;           /* how many primes found         */
static int            primes_built = 0;         /* lazy init flag                */

/* --------- helper: build prime table up to LIMIT --------- */
static void build_primes(void)
{
    if (primes_built) return;
    const uint32_t LIMIT = 1000000;            /* √max value we will test     */
    uint8_t* comp = (uint8_t*)calloc(LIMIT + 1, 1);
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!comp[i]) {
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= LIMIT)
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    comp[j] = 1;
        }
    free(comp);
    primes_built = 1;
}

/* --------- helper: compute Ω(n) for n <= 2^32 --------- */
static uint8_t omega_uint32(uint32_t n)
{
    uint8_t cnt = 0;
    uint32_t tmp = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
    }
    if (tmp > 1) ++cnt;
    return cnt;
}

/* --------- ensure a(prefix_len) >= target --------- */
static void ensure_prefix(size_t target)
{
    if (prefix_len >= target) return;

    /* enlarge prefix array if needed */
    if (!prefix) {
        prefix = (seq_t*)malloc(prefix_capacity * sizeof(seq_t));
    }
    while (prefix_len < target) {
        if (prefix_len >= prefix_capacity) {
            prefix_capacity *= 2;
            prefix = (seq_t*)realloc(prefix, prefix_capacity * sizeof(seq_t));
        }

        /* test next_number for Ω = 6 */
        if (omega_uint32((uint32_t)next_number) == 6) {
            cumulative += (seq_t)next_number;
            prefix[prefix_len++] = cumulative;
        }
        ++next_number;
    }
}

/* --------- public entry point --------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range handling */

    /* initialise primes once */
    if (!primes_built) build_primes();

    /* compute up to requested index */
    ensure_prefix((size_t)n);

    /* index is 1‑based, array 0‑based */
    return prefix[(size_t)n - 1];
}
