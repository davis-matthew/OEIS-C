/*
 * A239656  First differences of sphenic numbers, cf. A007304.
 * https://oeis.org/A239656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sphenic(n+1) - sphenic(n) * where sphenic(k) is the k-th
 *            positive integer that is the product * of three distinct primes,
 *            each appearing to the first power
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers ----------------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage ------------------------------------------------------------ */
static seq_t *sphenics = NULL;   /* sphenics[0] = 30, sphenics[1] = 42, …   */
static size_t s_cnt   = 0;       /* number of elements stored so far       */
static size_t s_cap   = 0;       /* allocated capacity for *sphenics       */

static uint32_t *primes   = NULL; /* prime table                           */
static size_t    p_cnt   = 0;     /* number of primes stored                */
static size_t    p_cap   = 0;     /* allocated capacity for *primes         */

static idx_t last_checked = 1;    /* integer last tested for sphenicity    */

/* Grow an array of seq_t --------------------------------------------------- */
static void grow_seq_array(size_t *cap, size_t need, seq_t **arr)
{
    if (*cap < need)
    {
        size_t new_cap = (*cap == 0) ? 32 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Grow the prime array ----------------------------------------------------- */
static void grow_prime_array(void)
{
    if (p_cap <= p_cnt)
    {
        size_t new_cap = (p_cap == 0) ? 16 : p_cap << 1;
        primes = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        p_cap = new_cap;
    }
}

/* Insert a prime into the primes table ------------------------------------- */
static void insert_prime(uint32_t p)
{
    grow_prime_array();
    primes[p_cnt++] = p;
}

/* Ensure that primes[0 .. p_cnt-1] contains all primes <= limit.           */
static void ensure_primes_up_to(uint32_t limit)
{
    if (p_cnt == 0)               /* initialise with first prime 2         */
    {
        insert_prime(2);
        if (limit < 2) return;
    }

    uint32_t cand = primes[p_cnt-1] + 1;
    while (primes[p_cnt-1] <= limit)
    {
        bool is_p = true;
        for (size_t i = 0; i < p_cnt &&
             (uint64_t)primes[i] * primes[i] <= cand; ++i)
        {
            if (cand % primes[i] == 0) { is_p = false; break; }
        }
        if (is_p) insert_prime(cand);
        ++cand;
    }
}

/* Test whether a number is sphenic (product of three distinct primes).      */
static bool is_sphenic(uint32_t m)
{
    if (m < 30) return false;           /* smallest sphenic is 30            */

    ensure_primes_up_to(m);             /* primes up to m (may include one >m) */

    int distinct = 0;
    uint32_t tmp   = m;

    for (size_t i = 0; i < p_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i)
    {
        uint32_t p = primes[i];
        if (tmp % p == 0)
        {
            int cnt = 0;
            while (tmp % p == 0) { ++cnt; tmp /= p; }
            if (cnt != 1) return false; /* exponent not 1                  */
            ++distinct;
            if (distinct > 3) return false;
        }
    }
    if (tmp > 1) {                       /* remaining prime factor          */
        ++distinct;
    }
    return (distinct == 3);
}

/* Ensure that the sphenics array contains at least 'needed' elements        */
static void ensure_sphenics(idx_t needed)
{
    if (s_cnt >= needed) return;

    while (s_cnt < needed)
    {
        ++last_checked;                     /* integer to test */
        if (last_checked > UINT32_MAX) break;/* safety guard   */

        uint32_t m = (uint32_t)last_checked;
        if (is_sphenic(m))
        {
            grow_seq_array(&s_cap, s_cnt + 1, &sphenics);
            sphenics[s_cnt++] = (seq_t)m;
        }
    }
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n<1, return 0   */

    /* Need the (n+1)-th sphenic number and the n-th one (1‑based).          */
    ensure_sphenics(n + 1);

    /* a(n) = sphenics[n] - sphenics[n-1]   (array indices 0‑based).         */
    return sphenics[n] - sphenics[n - 1];
}
