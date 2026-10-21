/*
 * A097232  Starting primes for which there are no solutions in A097231.
 * https://oeis.org/A097232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A097232 – starting primes for which no 3×3 arrangement
 *  of the block of 9 consecutive primes can have all row, column,
 *  and diagonal sums prime.
 *
 *  The function a(idx_t n) returns the n‑th such prime
 *  (a(1) = 2, a(2) = 23, …).  It is computed by generating
 *  primes incrementally and testing each 9‑prime block with a
 *  back‑tracking search that prunes by checking row sums on
 *  the fly.  The results are memoised in a static failure array
 *  so that calls with arbitrary n are fast and order‑independent.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- deterministic Miller–Rabin for 64‑bit ---------- */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* write n-1 as d * 2^s */
    uint64_t d = n-1, s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = 1, base = a, exp = d, mod = n;
        while (exp) {
            if (exp & 1) x = (uint64_t)((__int128)x * base % mod);
            base = (uint64_t)((__int128)base * base % mod);
            exp >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool fail = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (uint64_t)((__int128)x * x % mod);
            if (x == n-1) { fail = false; break; }
        }
        if (fail) return false;
    }
    return true;
}

/* ---------- integer sqrt for uint64_t (no <math.h>) ---------- */
static uint64_t isqrt64(uint64_t n)
{
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;          /* largest power of 4 <= n */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* ---------- dynamic prime cache (uint64_t) ---------- */
static uint64_t *prime_arr = NULL;
static size_t prime_cnt = 0, prime_cap = 0;

static void ensure_prime_cap(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t*)realloc(prime_arr, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    prime_arr = tmp;
    prime_cap = new_cap;
    if (prime_cnt == 0) { prime_arr[0] = 2; prime_cnt = 1; }
}

static void gen_next_prime(void)
{
    if (prime_cnt == 0) { prime_arr[0] = 2; prime_cnt = 1; return; }
    uint64_t cand = prime_arr[prime_cnt-1] + 1;
    if ((cand & 1ULL) == 0) cand++;          /* make odd */
    for (;;) {
        uint64_t limit = isqrt64(cand);
        bool ok = true;
        for (size_t i = 0; i < prime_cnt && prime_arr[i] <= limit; ++i) {
            if (cand % prime_arr[i] == 0) { ok = false; break; }
        }
        if (ok) { prime_arr[prime_cnt++] = cand; return; }
        cand += 2;
    }
}

/* ---------- dynamic failure cache (seq_t) ---------- */
static seq_t *fail_arr = NULL;
static size_t fail_cnt = 0, fail_cap = 0;

static void ensure_fail_cap(size_t need)
{
    if (fail_cap >= need) return;
    size_t new_cap = fail_cap ? fail_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(fail_arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    fail_arr = tmp;
    fail_cap = new_cap;
}

/* ---------- backtracking search for admissible Latin square ---------- */
static bool backtrack(uint64_t const p9[9], uint64_t cells[9], uint64_t used, int pos)
{
    if (pos == 9) {
        /* columns */
        if (!isPrime64(cells[0]+cells[3]+cells[6])) return false;
        if (!isPrime64(cells[1]+cells[4]+cells[7])) return false;
        if (!isPrime64(cells[2]+cells[5]+cells[8])) return false;
        /* diagonals */
        if (!isPrime64(cells[0]+cells[4]+cells[8])) return false;
        if (!isPrime64(cells[2]+cells[4]+cells[6])) return false;
        return true;
    }
    for (int i = 0; i < 9; ++i) {
        if (used & (1ULL<<i)) continue;
        uint64_t nused = used | (1ULL<<i);
        cells[pos] = p9[i];
        /* prune after completing a row */
        if (pos == 2 && !isPrime64(cells[0]+cells[1]+cells[2])) continue;
        if (pos == 5 && !isPrime64(cells[3]+cells[4]+cells[5])) continue;
        if (pos == 8 && !isPrime64(cells[6]+cells[7]+cells[8])) continue;
        if (backtrack(p9, cells, nused, pos+1)) return true;
    }
    return false;
}

static bool has_valid_arrangement(uint64_t const p9[9])
{
    uint64_t cells[9];
    return backtrack(p9, cells, 0ULL, 0);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0U) return 0;                       /* offset starts at 1 */

    static idx_t current_start = 0U;             /* index of current 9‑prime block */

    /* generate failure set until we have at least n entries */
    while (fail_cnt < (size_t)n) {
        /* ensure we have 9 primes starting at current_start */
        size_t needed = current_start + 9;
        ensure_prime_cap(needed);
        while (prime_cnt < needed) gen_next_prime();

        uint64_t block[9];
        for (int i = 0; i < 9; ++i) block[i] = prime_arr[current_start + i];

        if (!has_valid_arrangement(block)) {
            ensure_fail_cap(fail_cnt + 1);
            fail_arr[fail_cnt++] = (seq_t)block[0];
        }

        ++current_start;
    }

    return fail_arr[(size_t)n - 1];
}
