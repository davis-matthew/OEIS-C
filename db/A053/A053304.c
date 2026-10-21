/*
 * A053304  Number of 7 X 7 binary matrices with n=0..49 ones up to row and
 *          column permutations.
 * https://oeis.org/A053304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = Number of 7×7 binary matrices with n ones, up to row and
 *            column permutations
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    uint8_t a[8];   /* cycle counts for lengths 1..7 */
    idx_t permcnt;  /* number of permutations with this cycle type */
} partition_t;

/* 7!  */
static const idx_t FACT7 = 5040;
static partition_t *parts = NULL;
static size_t partcnt = 0;
static seq_t seq_table[50];
static bool initialized = false;

/* gcd and lcm for small integers */
static unsigned gcd_uint(unsigned a, unsigned b) {
    while (b) {
        unsigned t = b;
        b = a % b;
        a = t;
    }
    return a;
}
static unsigned lcm_uint(unsigned a, unsigned b) {
    return a / gcd_uint(a, b) * b;
}

/* Generate all integer partitions of 7, storing cycle counts and permutation counts */
static void generate_parts_rec(int rem, int max_part, uint8_t *cur) {
    if (rem == 0) {
        /* compute permutation count: 7! / prod i^{a_i} a_i! */
        idx_t denom = 1;
        for (int len = 1; len <= 7; ++len) {
            if (!cur[len]) continue;
            idx_t a = cur[len];
            /* i^{a_i} */
            idx_t pow = 1;
            for (idx_t k = 0; k < a; ++k) pow *= len;
            /* a_i! */
            idx_t fact = 1;
            for (idx_t k = 2; k <= a; ++k) fact *= k;
            denom *= pow * fact;
        }
        idx_t permcnt = FACT7 / denom;
        partition_t *p = parts + partcnt++;
        for (int i = 1; i <= 7; ++i) p->a[i] = cur[i];
        p->permcnt = permcnt;
        return;
    }
    for (int k = 1; k <= rem && k <= max_part; ++k) {
        cur[k]++;
        generate_parts_rec(rem - k, k, cur);
        cur[k]--;
    }
}

static void init(void) {
    if (initialized) return;
    /* allocate space for partitions (max 15 partitions) */
    parts = (partition_t *)malloc(64 * sizeof(partition_t));
    uint8_t cur[8] = {0};
    generate_parts_rec(7, 7, cur);

    /* Compute sequence values for n = 0..49 */
    const int MAX_N = 49;
    unsigned __int128 sum_arr[50] = {0};

    for (size_t i = 0; i < partcnt; ++i) {
        const partition_t *p1 = &parts[i];
        for (size_t j = 0; j < partcnt; ++j) {
            const partition_t *p2 = &parts[j];
            /* DP over orbit choices */
            seq_t dp[50] = {0};
            dp[0] = 1;
            for (int len1 = 1; len1 <= 7; ++len1) {
                uint8_t c1 = p1->a[len1];
                if (!c1) continue;
                for (int len2 = 1; len2 <= 7; ++len2) {
                    uint8_t c2 = p2->a[len2];
                    if (!c2) continue;
                    int g = gcd_uint(len1, len2);
                    int l = lcm_uint(len1, len2);
                    int orbs = c1 * c2 * g;
                    for (int o = 0; o < orbs; ++o) {
                        for (int m = MAX_N; m >= l; --m)
                            dp[m] += dp[m - l];
                    }
                }
            }
            unsigned __int128 prod = (unsigned __int128)p1->permcnt *
                                    (unsigned __int128)p2->permcnt;
            for (int n = 0; n <= MAX_N; ++n) {
                sum_arr[n] += prod * (unsigned __int128)dp[n];
            }
        }
    }

    /* Final division by |S7|^2 = 7! * 7! */
    unsigned __int128 denom = (unsigned __int128)FACT7 * FACT7;
    for (int n = 0; n <= MAX_N; ++n) {
        seq_table[n] = (seq_t)(sum_arr[n] / denom);
    }
    initialized = true;
}

seq_t a(idx_t n) {
    init();
    if (n > 49) return 0;
    return seq_table[(size_t)n];
}
