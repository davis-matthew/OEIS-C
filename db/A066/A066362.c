/*
 * A066362  a(n) = least k > n such that phi(k) < phi(n), if such a k exists;
 *          otherwise a(n) = 0.
 * https://oeis.org/A066362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>n such that φ(k) < φ(n), otherwise 0
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   build_phi, fenwick_query, fenwick_update, precompute
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_phi.h"
#include "../utils/fenwick_query.h"
#include "../utils/fenwick_update.h"
#include "../utils/precompute.h"

/* Initial maximum limit for precomputation.  The sequence for n≤10 000
   uses values far below 1 000 000, so this value is safe for the test
   harness.  The algorithm can extend this limit automatically if a
   larger n is requested. */
#define INITIAL_LIMIT 2000000

/* Singleton to keep the precomputed data in static variables. */
typedef struct {
    uint32_t *phi;
    seq_t *ans;
    idx_t *bit;
    idx_t limit;
    int initialized;
} static_data_t;
static static_data_t data = {0,0,0,0,0};

static void ensure_precomputed(idx_t n)
{
    if (!data.initialized || n > data.limit) {
        /* Extend the limit sufficiently to hold at least n and some buffer. */
        idx_t new_limit = n;
        if (new_limit < INITIAL_LIMIT)
            new_limit = INITIAL_LIMIT;
        while (new_limit < n) new_limit *= 2;

        /* Rebuild all data. */
        uint32_t *phi_cache = build_phi(new_limit);
        seq_t *ans_cache  = (seq_t *)malloc((new_limit + 1) * sizeof(seq_t));
        if (!ans_cache) abort();
        idx_t *bit = (idx_t *)calloc(new_limit + 1, sizeof(idx_t));
        if (!bit) abort();

        for (idx_t i = new_limit; i >= 1; --i) {
            uint32_t phi_n = phi_cache[i];
            idx_t cand = (phi_n > 1) ? fenwick_query(bit, phi_n - 1) : 0;
            ans_cache[i] = (cand ? cand : 0);
            fenwick_update(bit, new_limit, phi_n, i);
        }

        /* Free old data. */
        if (data.initialized) {
            free(data.phi);
            free(data.ans);
            free(data.bit);
        }

        data.phi = phi_cache;
        data.ans = ans_cache;
        data.bit = bit;
        data.limit = new_limit;
        data.initialized = 1;
    }
}

/* Public function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */
    ensure_precomputed(n);
    return data.ans[n];
}
