/*
 * A099501  Least number of members in a subset of integers in the interval
 *          [n^2+1, (n+1)^2-1] whose product is twice a square.
 * https://oeis.org/A099501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least number of members in a subset of the interval [n^2+1,(n+1)^2-1]
   whose product is twice a square.
   The routine first searches for a single element 2*s^2,
   then for a pair by comparing odd squarefree parts
   and the parity of the exponent of 2.
   If neither is found it returns 3 (the conjectured upper bound). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --- helper: integer square‑root for idx_t --------------------------------- */
static idx_t isqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;          /* safe for x <= 1e8 */
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;                          /* floor(sqrt(x)) */
}

/* --- prime sieve up to 10000 --------------------------------------------- */
#define MAX_PRIME 10000
static int  primes[1229];
static size_t primes_cnt = 0;
static bool  primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    bool *is_comp = (bool*)calloc(MAX_PRIME + 1, sizeof(bool));
    if (!is_comp) { primes_ready = false; return; }
    size_t cnt = 0;
    for (idx_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            primes[cnt++] = (int)i;
            if (i * i <= MAX_PRIME) {
                for (idx_t j = i * i; j <= MAX_PRIME; j += i)
                    is_comp[j] = true;
            }
        }
    }
    primes_cnt  = cnt;
    primes_ready = true;
    free(is_comp);
}

/* --- entry for a single number ------------------------------------------------ */
struct Entry {
    uint64_t odd;          /* squarefree odd part   */
    unsigned char parity;  /* parity of exponent of 2 (0 even, 1 odd) */
};

static int cmp_entry(const void *p1, const void *p2)
{
    const struct Entry *a = (const struct Entry*)p1;
    const struct Entry *b = (const struct Entry*)p2;
    if (a->odd < b->odd) return -1;
    if (a->odd > b->odd) return  1;
    return 0;
}

/* --- main routine -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialize primes once */
    init_primes();

    /* interval bounds as idx_t (<= 1e8 for n <= 10000) */
    idx_t low  = n * n + 1;
    idx_t high = (n + 1) * (n + 1) - 1;

    /* ---- 1. single element? 2*s^2 in interval --------------------------- */
    idx_t ceil_half_low = (low + 1) / 2;      /* ceil(low/2)   */
    idx_t k_min = isqrt(ceil_half_low);
    if (k_min * k_min < ceil_half_low) ++k_min;

    idx_t k_max = isqrt(high / 2);
    if (k_min <= k_max) {
        return (seq_t)1;
    }

    /* ---- 2. search for a pair ------------------------------------------ */
    size_t len = (size_t)(high - low + 1);    /* at most 2*n <= 20000 */
    struct Entry *arr = (struct Entry*)malloc(len * sizeof(struct Entry));
    if (!arr) {
        return (seq_t)3;    /* fallback on allocation failure */
    }

    size_t pos = 0;
    for (idx_t m = low; m <= high; ++m, ++pos) {
        uint64_t x = (uint64_t)m;
        /* parity of 2 */
        unsigned parity = 0;
        while ((x & 1ULL) == 0) {
            parity ^= 1;
            x >>= 1;
        }

        /* odd squarefree part */
        uint64_t odd_sf = 1;
        for (size_t pi = 0; pi < primes_cnt && (uint64_t)primes[pi] * primes[pi] <= x; ++pi) {
            int p = primes[pi];
            if (x % p == 0) {
                int cnt = 0;
                while (x % p == 0) {
                    x /= p;
                    ++cnt;
                }
                if (cnt & 1) {
                    odd_sf *= (uint64_t)p;
                }
            }
        }
        if (x > 1) {
            odd_sf *= x;      /* remaining prime factor has exponent 1 */
        }

        arr[pos].odd     = odd_sf;
        arr[pos].parity  = (unsigned char)parity;
    }

    /* sort entries by odd part */
    qsort(arr, len, sizeof(struct Entry), cmp_entry);

    /* scan for entries with both parities */
    for (size_t i = 0; i < len; ) {
        size_t j = i + 1;
        bool has_even = false, has_odd = false;
        if (arr[i].parity == 0) has_even = true; else has_odd = true;
        while (j < len && arr[j].odd == arr[i].odd) {
            if (arr[j].parity == 0) has_even = true; else has_odd = true;
            ++j;
        }
        if (has_even && has_odd) {
            free(arr);
            return (seq_t)2;
        }
        i = j;
    }

    free(arr);
    return (seq_t)3;
}
