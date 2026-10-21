/*
 * A074126  Group the composite numbers so that the sum of the n-th group is a
 *          multiple of the n-th prime: (4), (6), (8, 9, 10, 12, 14, 15, 16,
 *          18, 20, 21, 22), (24, 25), (26, 27, 28, 30, 32), (33, 34, ...),
 *          ... Sequence gives the number of terms in each group.
 * https://oeis.org/A074126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074126: groups of consecutive composite numbers are formed so that the
   sum of the numbers in the ith group is a multiple of the ith prime number.
   a(n) returns the number of terms in the nth group.  */
#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build system.  They are unsigned integer
   types of appropriate size (e.g. uint64_t, unsigned __int128, or arbitrary
   precision).  No other declarations of them are required.  */

/* ---------- prime infrastructure ------------------------------------ */

/* dynamic array of the primes we have generated; prime_list[0] = 2 */
static idx_t *prime_list = NULL;
static size_t prime_cnt = 0;   /* number of primes currently stored   */
static size_t prime_cap = 0;   /* allocated capacity of prime_list   */

/* floor sqrt for idx_t (binary search) */
static idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* ensure prime_list has capacity for at least 'need' primes */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap == 0) {
        prime_cap = (need > 64) ? need : 64;
        prime_list = (idx_t *)malloc(prime_cap * sizeof(idx_t));
    } else if (need > prime_cap) {
        prime_cap *= 2;
        prime_list = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
    }
}

/* generate the next prime and append it to prime_list */
static void generate_next_prime(void)
{
    idx_t cand = (prime_cnt == 0 ? 2 : prime_list[prime_cnt - 1] + 1);
    for (;;) {
        int isp = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if ((idx_t)p * p > cand) break;
            if (cand % p == 0) { isp = 0; break; }
        }
        if (isp) {
            if (prime_cnt == prime_cap) {
                prime_cap = (prime_cap == 0) ? 64 : prime_cap * 2;
                prime_list = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
            }
            prime_list[prime_cnt++] = cand;
            return;
        }
        ++cand;
    }
}

/* make sure we have at least 'cnt' primes (1‑based count) */
static void ensure_prime_count(size_t cnt)
{
    while (prime_cnt < cnt) generate_next_prime();
}

/* simple primality test using the current prime list */
static int isPrime(idx_t n)
{
    if (n < 2) return 0;
    if (n == 2) return 1;
    if ((n & 1) == 0) return 0;
    idx_t lim = isqrt_idx(n);
    while (prime_cnt == 0 || prime_list[prime_cnt - 1] < lim)
        generate_next_prime();
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > lim) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

/* ---------- sequence storage ---------------------------------------- */

static seq_t *aseq = NULL;   /* aseq[0] holds a(1) */
static size_t acnt = 0;      /* number of computed terms */
static size_t acap = 0;      /* allocated capacity */

/* ensure space for at least 'need' terms */
static void ensure_a_cap(size_t need)
{
    if (acap == 0) {
        acap = (need > 64) ? need : 64;
        aseq = (seq_t *)malloc(acap * sizeof(seq_t));
    } else if (need > acap) {
        acap *= 2;
        aseq = (seq_t *)realloc(aseq, acap * sizeof(seq_t));
    }
}
  
/* next composite number to test (global state across calls) */
static idx_t next_composite = 4;

/* ------------------------------------------------------------ */
/* Function implementing the sequence                                       */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for indices < 1 */

    /* compute terms up to n (memoised) */
    while (acnt < n) {
        idx_t group_idx = acnt + 1;        /* 1‑based index of the new group */
        ensure_prime_count((size_t)group_idx);
        idx_t prime = prime_list[group_idx - 1];

        idx_t group_size = 0;
        idx_t remainder = 0;   /* (sum of group modulo prime) */

        while (1) {
            /* skip prime numbers */
            while (isPrime(next_composite)) ++next_composite;

            /* add this composite to the group */
            ++group_size;
            remainder = (remainder + (next_composite % prime)) % prime;

            if (remainder == 0) {        /* group finished */
                ++next_composite;        /* advance to next number for future groups */
                break;
            }
            ++next_composite;
        }

        ensure_a_cap(acnt + 1);
        aseq[acnt++] = (seq_t)group_size;
    }

    return aseq[n - 1];
}
