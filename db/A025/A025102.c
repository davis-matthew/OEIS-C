/*
 * A025102  s(1)s(n) + s(2)s(n-1) + ... + s(k)s(n-k+1), where k = [ n/2 ], s =
 *          (composite numbers).
 * https://oeis.org/A025102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} s(i)*s(n-i+2), where s(i) is the i‑th
 *            composite number
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *comps = NULL;          /* 1‑indexed array of composites */
static size_t comps_count = 0;       /* number of composites stored */
static bool comps_initialized = false;

/* Build a list of composites up to a fixed range (big enough for n ≤ 10000). */
static void init_composites(void) {
    if (comps_initialized) return;

    const idx_t LIMIT_VAL = 200000;        /* sufficient for all requested n */

    bool *isPrime = (bool*)malloc((LIMIT_VAL + 1) * sizeof(bool));
    if (!isPrime) return;                  /* out of memory – undefined but unlikely */

    for (idx_t i = 0; i <= LIMIT_VAL; ++i) isPrime[i] = true;
    isPrime[0] = isPrime[1] = false;
    for (idx_t p = 2; p * p <= LIMIT_VAL; ++p) {
        if (isPrime[p]) {
            for (idx_t m = p * p; m <= LIMIT_VAL; m += p)
                isPrime[m] = false;
        }
    }

    /* Count composites to allocate the array */
    size_t count = 0;
    for (idx_t num = 4; num <= LIMIT_VAL; ++num)
        if (!isPrime[num]) ++count;

    comps = (seq_t*)malloc((count + 1) * sizeof(seq_t));
    if (!comps) { free(isPrime); return; }

    size_t idx = 1;
    for (idx_t num = 4; num <= LIMIT_VAL; ++num) {
        if (!isPrime[num]) comps[idx++] = (seq_t)num;
    }
    comps_count = idx - 1;
    comps_initialized = true;
    free(isPrime);
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n < 1, return 0 */

    init_composites();

    /* Ensure there are enough composites; should always hold with LIMIT_VAL */
    if (comps_count < (size_t)(n + 1)) {
        return (seq_t)0;
    }

    idx_t k = (n + 1) / 2;
    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= k; ++i) {
        sum += comps[i] * comps[n - i + 2];
    }
    return sum;
}
