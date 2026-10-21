/*
 * A066900  a(n) = max( {k : phi(k)==n (mod(k)) } ).
 * https://oeis.org/A066900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max { k : n % k == phi(k) }
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define LIMIT_K 200000 /* maximum k considered, enough for n ≤ 10000 */
#define LIMIT_N 200000 /* maximum n precomputed for */

static uint32_t *phi_arr = NULL;  /* phi values for k = 0 … LIMIT_K */
static seq_t   *a_cache  = NULL;  /* precomputed a(n) values */
static int initialized = 0;

/* compute phi(k) for k up to LIMIT_K using linear sieve */
static void init_phi(void)
{
    phi_arr = (uint32_t*)malloc((LIMIT_K + 1) * sizeof(uint32_t));
    if (!phi_arr) return;                     /* allocation failure */

    uint32_t *primes = (uint32_t*)malloc((LIMIT_K / 10 + 10) * sizeof(uint32_t));
    if (!primes) return;
    size_t  pcnt = 0, pcap = LIMIT_K / 10 + 10;
    uint8_t *is_comp = (uint8_t*)calloc(LIMIT_K + 1, sizeof(uint8_t));
    if (!is_comp) return;

    phi_arr[0] = 0;
    phi_arr[1] = 1;

    for (idx_t i = 2; i <= LIMIT_K; ++i)
    {
        if (!is_comp[i]) {
            if (pcnt == pcap) {               /* enlarge prime list if needed */
                pcap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(primes, pcap * sizeof(uint32_t));
                if (!tmp) break;
                primes = tmp;
            }
            primes[pcnt++] = (uint32_t)i;
            phi_arr[i] = (uint32_t)(i - 1);
        }

        for (size_t j = 0; j < pcnt; ++j)
        {
            idx_t p = (idx_t)primes[j];
            idx_t ip = i * p;
            if (ip > LIMIT_K) break;
            is_comp[ip] = 1;
            if (i % p == 0) {
                phi_arr[ip] = phi_arr[i] * (uint32_t)p;
                break;
            } else {
                phi_arr[ip] = phi_arr[i] * (uint32_t)(p - 1);
            }
        }
    }

    free(is_comp);
    free(primes);
}

/* compute a(n) for all n up to LIMIT_N */
static void init_cache(void)
{
    init_phi();
    a_cache = (seq_t*)calloc((LIMIT_N + 1) * sizeof(seq_t), 1);
    if (!a_cache) return;

    for (idx_t k = 2; k <= LIMIT_K; ++k) {
        uint32_t r = phi_arr[k];
        if (r > (uint32_t)LIMIT_N) continue;
        for (idx_t n = r; n <= LIMIT_N; n += k) {
            seq_t k_val = (seq_t)k;
            if (k_val > a_cache[n]) a_cache[n] = k_val;
        }
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) init_cache();
    if (n <= (idx_t)LIMIT_N) return a_cache[n];
    /* If n exceeds pre‑computed range we fall back to brute force on demand.
       This is a very small overhead for the unlikely case. */
    seq_t best = 0;
    for (idx_t k = 2; k <= LIMIT_K; ++k) {
        uint32_t r = phi_arr[k];
        if (r > (uint32_t)n) continue;            /* for k > n only r == n is relevant */
        if ((n % k) == r && (seq_t)k > best) best = (seq_t)k;
    }
    return best;
}
