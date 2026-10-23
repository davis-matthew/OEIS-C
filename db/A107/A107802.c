/*
 * A107802  a(1) = prime(2), for n >= 2, a(n) is the smallest prime not
 *          previously used which contains a digit from a(n-1).
 * https://oeis.org/A107802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 3. For n ≥ 2, a(n) is the smallest unused prime
 *            containing at least one digit of a(n − 1). The implementation
 *            uses a sieve to generate all primes up to a fixed limit,
 *            pre‑computes the digit mask of each prime, and keeps linked
 *            lists of prime indices for each digit. For a step it selects
 *            th...
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const uint32_t SIEVE_LIMIT = 5000000;   /* enough for n up to ~10 000 */
static uint64_t *primes = NULL;
static uint16_t *prime_mask = NULL;
static bool *used = NULL;
static int *digit_list[10];
static size_t digit_cnt[10];
static size_t digit_pos[10];
static size_t primes_cnt = 0;
static seq_t *seq = NULL;
static size_t seq_len = 0, seq_cap = 0;
static uint16_t last_mask = 0;
static bool initialized = false;

static inline uint16_t digit_mask(uint64_t x) {
    uint16_t m = 0;
    if (x == 0) return 1;                 /* mask for digit 0 */
    while (x) {
        m |= 1u << (uint16_t)(x % 10);
        x /= 10;
    }
    return m;
}

static void init(void) {
    if (initialized) return;
    initialized = true;

    /* 1. Sieve */
    char *is_prime = (char*)malloc(SIEVE_LIMIT + 1);
    for (uint32_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[(size_t)j] = 0;

    /* estimate count */
    size_t est = SIEVE_LIMIT / 10;
    primes = (uint64_t*)malloc(est * sizeof(uint64_t));
    prime_mask = (uint16_t*)malloc(est * sizeof(uint16_t));
    /* temporary array to store digit lists before knowing exact sizes */
    for (int d = 0; d < 10; ++d) {
        digit_list[d] = (int*)malloc(est * sizeof(int));
        digit_cnt[d] = 0;
    }

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_prime[i]) continue;
        if (primes_cnt == est) {
            est *= 2;
            primes = (uint64_t*)realloc(primes, est * sizeof(uint64_t));
            prime_mask = (uint16_t*)realloc(prime_mask, est * sizeof(uint16_t));
            for (int d = 0; d < 10; ++d) {
                digit_list[d] = (int*)realloc(digit_list[d], est * sizeof(int));
            }
        }
        primes[primes_cnt] = i;
        uint16_t m = digit_mask(i);
        prime_mask[primes_cnt] = m;
        for (int d = 0; d < 10; ++d)
            if (m & (1u << d)) {
                digit_list[d][digit_cnt[d]++] = (int)primes_cnt;
            }
        ++primes_cnt;
    }
    free(is_prime);

    /* mark used array */
    used = (bool*)calloc(primes_cnt, sizeof(bool));

    /* initialise sequence with first term a(1)=3 */
    seq_cap = 256;
    seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;
    /* locate prime 3 */
    size_t idx3 = 1; /* primes[1]==3 with 2 as first prime */
    seq[seq_len++] = (seq_t)3;
    used[idx3] = true;
    last_mask = prime_mask[(int)idx3];

    /* initialise digit positions */
    for (int d = 0; d < 10; ++d) digit_pos[d] = 0;
}

/* ensure sequence has at least n terms */
static void prepare(idx_t n) {
    if (n == 0) return;
    if (seq_len >= (size_t)n) return;
    init();

    while (seq_len < (size_t)n) {
        int best_idx = -1;
        seq_t best_p = 0xFFFFFFFFFFFFFFFFULL;
        /* examine all digits present in last prime */
        for (int d = 0; d < 10; ++d) {
            if (!(last_mask & (1u << d))) continue;
            /* advance to unused */
            while (digit_pos[d] < digit_cnt[d] &&
                   used[digit_list[d][digit_pos[d]]]) {
                ++digit_pos[d];
            }
            if (digit_pos[d] == digit_cnt[d]) continue;
            int cand_idx = digit_list[d][digit_pos[d]];
            seq_t cand = primes[cand_idx];
            if (best_idx == -1 || cand < best_p) {
                best_idx = cand_idx;
                best_p = cand;
            }
        }
        if (best_idx < 0) {
            /* should not happen with a large enough sieve limit */
            return;
        }
        used[best_idx] = true;
        if (seq_cap <= seq_len + 1) {
            seq_cap = seq_cap ? seq_cap * 2 : 128;
            seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = best_p;
        last_mask = prime_mask[best_idx];
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    prepare(n);
    return seq[n-1];
}
