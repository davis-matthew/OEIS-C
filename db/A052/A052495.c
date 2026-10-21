/*
 * A052495  Take n-th prime p, let P = all primes having same digits; a(n) =
 *          q-p where q is smallest prime in P >p if q exists; otherwise a(n)
 *          = p-r where r is largest prime in P <p if r exists; otherwise a(n)
 *          = 0.
 * https://oeis.org/A052495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A052495
 *
 *  a(n) is the distance from the n‑th prime p to the nearest other prime
 *  that is a permutation of the decimal digits of p (using the same number
 *  of digits, so internal zeros are preserved).  If a larger such prime
 *  q exists, a(n)=q−p, otherwise if a smaller r exists, a(n)=p−r,
 *  otherwise a(n)=0.
 *
 *  The implementation builds a sieve of primes up to 2 000 000, stores
 *  them in an array, and for each call generates all distinct
 *  permutations of the digits of the requested prime and tests each
 *  permutation for primality using the sieve.  The best greater and
 *  lesser primes are recorded and the required difference returned.
 *
 *  Complexity:  O(1) for the one‑time sieve construction.
 *  For each a(n) the work is bounded by O(k!) where k is the number of
 *  digits of the n‑th prime (k ≤ 6 for n ≤ 10 000).  This easily meets
 *  the 2‑second per call requirement for all requested terms.
 */
/* clang-format off */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define PRIME_LIMIT 2000000   /* enough to contain at least the 10 000th prime */

/* ---- static data ---- */
static bool initialized = false;
static uint8_t *is_prime_table = NULL;   /* 0 = not prime, 1 = prime */
static int *primes = NULL;
static size_t primes_cnt = 0;

/* ---- helper functions ---- */
static void init_primes(void);
static bool is_prime_int(int v);
static void permute_rec(const int *digits, bool *used, int len, int depth,
                        int cur, int target, int *q, int *r);

/* ---- build sieve and list of primes ---- */
static void init_primes(void)
{
    if (initialized) return;
    initialized = true;

    is_prime_table = (uint8_t *)calloc(PRIME_LIMIT + 1, sizeof(uint8_t));
    if (!is_prime_table) exit(1);
    for (int i = 2; i <= PRIME_LIMIT; ++i) is_prime_table[i] = 1;
    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i) {
        if (is_prime_table[i]) {
            for (long long j = (long long)i * i; j <= PRIME_LIMIT; j += i)
                is_prime_table[j] = 0;
        }
    }

    size_t capacity = 200000;         /* enough for all primes ≤ 2 000 000 */
    primes = (int *)malloc(capacity * sizeof(int));
    if (!primes) exit(1);
    primes_cnt = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime_table[i]) {
            if (primes_cnt == capacity) {          /* expand storage if needed */
                capacity *= 2;
                int *tmp = (int *)realloc(primes, capacity * sizeof(int));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[primes_cnt++] = i;
        }
    }
}

/* ---- test small integer using the sieve ---- */
static bool is_prime_int(int v)
{
    if (v <= PRIME_LIMIT) return is_prime_table[v] != 0;
    return false;   /* should not reach for our use case */
}

/* ---- generate all distinct permutations of the digits ---- */
static void permute_rec(const int *digits, bool *used, int len, int depth,
                        int cur, int target, int *q, int *r)
{
    if (depth == len) {                     /* full number built */
        if (is_prime_int(cur)) {
            if (cur > target) {
                if (*q == -1 || cur < *q) *q = cur;
            } else if (cur < target) {
                if (*r == -1 || cur > *r) *r = cur;
            }
        }
        return;
    }

    for (int i = 0; i < len; ++i) {
        if (used[i]) continue;
        if (depth == 0 && digits[i] == 0) continue;   /* no leading zero */
        if (i > 0 && digits[i] == digits[i-1] && !used[i-1]) continue;  /* avoid duplicates */

        used[i] = true;
        permute_rec(digits, used, len, depth + 1, cur * 10 + digits[i],
                    target, q, r);
        used[i] = false;
    }
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    init_primes();                  /* ensure primes are ready */

    if (n == 0) return 0;           /* safety, though n ≥ 1 */
    size_t idx = (size_t)n - 1;
    if (idx >= primes_cnt) return 0; /* out of precomputed range */

    int p = primes[idx];             /* n-th prime */

    /* extract digits of p */
    int digits[10];
    int len = 0;
    int tmp = p;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }
    /* sort digits ascending to ease duplicate handling */
    for (int i = 0; i < len - 1; ++i)
        for (int j = 0; j < len - 1 - i; ++j)
            if (digits[j] > digits[j+1]) {
                int t = digits[j];
                digits[j] = digits[j+1];
                digits[j+1] = t;
            }

    bool used[10] = {0};
    int q = -1;   /* smallest prime > p */
    int r = -1;   /* largest prime < p */

    permute_rec(digits, used, len, 0, 0, p, &q, &r);

    if (q != -1) return (seq_t)(q - p);
    if (r != -1) return (seq_t)(p - r);
    return (seq_t)0;
}
