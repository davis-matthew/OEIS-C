/*
 * A228094  Triangle starting at row 3 read by rows of the number of
 *          permutations in the n-th Dihedral group which are the product of k
 *          disjoint cycles, d(n,k), n >= 3, 1 <= k <= n.
 * https://oeis.org/A228094
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   d(n,k) = sum_{k|n} φ(n/k) + reflection contribution * a(n) is
 *            the flattened triangle where row N (N>=3) contains * the values
 *            d(N,1), d(N,2), …, d(N,N)
 * Range:     u8 n <= 9441, u16 n <= 9441, u32 n <= 9441, u64 n <= 9441,
 *            u128 n <= 9441, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- auxiliary: sieve for primes up to LIMIT ---------- */
static unsigned int *primes = NULL;
static size_t primes_cnt = 0;
#define PRIMES_LIMIT 10000   /* enough for N <= 10000 */

static void init_primes(void)
{
    if (primes) return;
    size_t size = PRIMES_LIMIT + 1;
    unsigned char *mark = (unsigned char *)calloc(size, 1);
    if (!mark) return;
    for (size_t i = 2; i <= PRIMES_LIMIT; ++i) {
        if (!mark[i]) {
            primes = (unsigned int *)realloc(primes, sizeof(unsigned int) * (++primes_cnt));
            primes[primes_cnt - 1] = (unsigned int)i;
            if (i * i <= PRIMES_LIMIT) {
                for (size_t j = i * i; j <= PRIMES_LIMIT; j += i)
                    mark[j] = 1;
            }
        }
    }
    free(mark);
}

/* ---------- totient using the pre‑computed primes ---------- */
static seq_t totient(uint64_t m)
{
    if (m == 0) return 0;
    seq_t res = (seq_t)m;
    uint64_t n = m;
    init_primes();
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            res = res / p * (p - 1);
        }
    }
    if (n > 1) { // remaining prime factor
        res = res / n * (n - 1);
    }
    return res;
}

/* ---------- dihedral group cycle count d(N,k) ---------- */
static seq_t dihedral_count(uint64_t N, uint64_t k)
{
    seq_t res = 0;
    if (k != 0 && N % k == 0) {
        res += totient(N / k);
    }
    /* reflection contribution */
    if (N & 1) {            /* N odd */
        if (k == (N + 1) / 2) res += (seq_t)N;
    } else {                /* N even */
        if (k == N / 2)      res += (seq_t)(N / 2);
        if (k == (N + 2) / 2)res += (seq_t)(N / 2);
    }
    return res;
}

/* ---------- find row number N for a given flattened position p ---------- */
static uint64_t find_row(uint64_t p)
{
    uint64_t lo = 3, hi = 3;
    while (hi * (hi + 1) / 2 - 3 < p) {
        hi <<= 1; /* double until enough */
    }
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t cum = mid * (mid + 1) / 2 - 3;
        if (cum >= p)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* ---------- public function a(n) ---------- */
seq_t a(idx_t n)
{
    if (n < 3) return 0;            /* below offset, undefined */
    uint64_t p = (uint64_t)n - 2;    /* position in the flattened triangle, 1‑based */
    uint64_t N = find_row(p);       /* row number */
    uint64_t prev_cum = (N - 1) * N / 2 - 3; /* terms before row N */
    uint64_t k = p - prev_cum;      /* column within the row, 1‑based */
    return dihedral_count(N, k);
}
