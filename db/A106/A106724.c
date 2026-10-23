/*
 * A106724  Largest of eight consecutive primes whose sum of digits is prime.
 * https://oeis.org/A106724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the largest prime among any run of 8 consecutive primes
 *            whose digit sums are all prime (OEIS A106724). The sequence
 *            starts with a(1)=3307, a(2)=4447, …
 * Range:     u8 none, u16 n <= 5, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Caveats for the build environment:
   * seq_t and idx_t are integer types supplied by the build.
   * The code is pure C (C11) but is also valid C++17.
   * Only standard headers are used.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/is_prime_int.h"

/* ===== State kept between calls ===== */
static struct {
    /* small primes (≤ 200 000) used for segmented sieving */
    unsigned *small_primes;
    size_t sp_cnt, sp_cap;

    /* terms of the sequence */
    unsigned *terms;
    size_t terms_cnt, terms_cap;

    /* segmented sieve */
    unsigned seg_size;      /* e.g. 1 048 576 */
    unsigned cur_low;       /* current segment lower bound (inclusive) */

    /* sliding window of last 8 primes' digit‑sum‑prime flags */
    bool win[8];
    unsigned win_pos;       /* next position to overwrite */
    unsigned win_size;      /* how many flags stored (≤8) */
    unsigned win_count;     /* how many of those 8 flags are true */

    /* table: is_prime[sum] for sums up to 81 */
    bool is_prime_sum[82];
} S = {0};

/* ---------- Initialize primality table for digit sums ---------- */
static void init_prime_sum_table(void) {
    for (int i = 0; i < 82; ++i) {
        S.is_prime_sum[i] = is_prime_int(i);
    }
}

/* ---------- Build array of small primes (≤ 200 000) ---------- */
static void build_small_primes(void) {
    const unsigned LIMIT = 200000;
    unsigned *sieve = (unsigned *)calloc(LIMIT + 1, sizeof(unsigned));
    if (!sieve) exit(EXIT_FAILURE);
    for (unsigned p = 2; p * p <= LIMIT; ++p)
        if (!sieve[p])
            for (unsigned q = p * p; q <= LIMIT; q += p)
                sieve[q] = 1;
    for (unsigned p = 2; p <= LIMIT; ++p)
        if (!sieve[p]) {
            if (S.sp_cap == S.sp_cnt) {
                S.sp_cap = S.sp_cap ? S.sp_cap * 2 : 16;
                S.small_primes = (unsigned *)realloc(S.small_primes,
                        S.sp_cap * sizeof(unsigned));
            }
            S.small_primes[S.sp_cnt++] = p;
        }
    free(sieve);
}

/* ---------- Digit sum of an unsigned integer ---------- */
static unsigned digit_sum(unsigned x) {
    unsigned s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* ---------- Process a new prime: update sliding window ---------- */
static void process_prime(unsigned p) {
    bool ds_prime = S.is_prime_sum[digit_sum(p)];
    if (S.win_size < 8) {
        S.win[S.win_pos] = ds_prime;
        if (ds_prime) S.win_count++;
        S.win_size++;
    } else {
        bool old = S.win[S.win_pos];
        if (old) S.win_count--;
        S.win[S.win_pos] = ds_prime;
        if (ds_prime) S.win_count++;
    }
    S.win_pos = (S.win_pos + 1) & 7;   /* modulo 8 */

    if (S.win_size == 8 && S.win_count == 8) {
        /* store term */
        if (S.terms_cap == S.terms_cnt) {
            S.terms_cap = S.terms_cap ? S.terms_cap * 2 : 16;
            S.terms = (unsigned *)realloc(S.terms,
                    S.terms_cap * sizeof(unsigned));
        }
        S.terms[S.terms_cnt++] = p;
    }
}

/* ---------- Generate next segment of primes ---------- */
static void generate_segment(void) {
    unsigned low = S.cur_low;
    unsigned high = low + S.seg_size;
    size_t seg_len = (size_t)high - (size_t)low;
    char *mark = (char *)malloc(seg_len);
    if (!mark) exit(EXIT_FAILURE);
    memset(mark, 1, seg_len);          /* assume all prime */

    unsigned sqrt_high = (unsigned)floor(sqrt((double)high));
    for (size_t i = 0; i < S.sp_cnt && S.small_primes[i] <= sqrt_high; ++i) {
        unsigned p = S.small_primes[i];
        unsigned start = (low + p - 1) / p * p;
        if (start < p * p) start = p * p;
        for (unsigned j = start; j < high; j += p)
            mark[j - low] = 0;
    }

    for (unsigned i = low; i < high; ++i) {
        if (i < 2) continue;
        if (mark[i - low])
            process_prime(i);
    }

    free(mark);
    S.cur_low = high;
}

/* ---------- One‑time init of global state ---------- */
static void init_state(void) {
    S.seg_size = 1 << 20;      /* 1 048 576 (≈1 MiB) */
    S.cur_low = 2;
    S.win_pos = S.win_size = S.win_count = 0;
    S.terms_cnt = 0; S.terms_cap = 0; S.terms = NULL;
    S.sp_cnt = 0;  S.sp_cap = 0;  S.small_primes = NULL;

    init_prime_sum_table();
    build_small_primes();
}

/* ======== The requested function ======== */
seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        init_state();
        initialized = true;
    }

    while (S.terms_cnt < n)
        generate_segment();

    return (seq_t)S.terms[n - 1];
}
