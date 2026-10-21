/*
 * A093331  Number of ternary necklaces of length n with no subsequence 00.
 * https://oeis.org/A093331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/n) * Σ_{d|n} φ(n/d) * B(d) where B(d) = number of
 *            ternary strings of length d with no consecutive zeros and not
 *            both first and last symbol zero. B(d) = T(d) – G(d–2), d ≥ 2;
 *            B(1) = 2; T(d) = total number of length‑d strings over {0,1,2}
 *            with no consecutive zero, G(L) = number of length‑...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ----------  cached tables ---------------- */
static uint64_t *phi_arr = NULL;   /* Euler totient   */
static seq_t   *T_arr   = NULL;   /* total strings   */
static seq_t   *G_arr   = NULL;   /* strings with first & last non‑zero */
static size_t  val_size = 0;      /* current table size */

/* ----------  table initialisation  --------- */
static void init_up_to(idx_t n)
{
    if ((size_t)n <= val_size) return;          /* already ready */

    size_t N = (size_t)n;
    phi_arr = (uint64_t*)realloc(phi_arr, (N + 1) * sizeof(uint64_t));
    T_arr   = (seq_t*)   realloc(T_arr,   (N + 1) * sizeof(seq_t));
    G_arr   = (seq_t*)   realloc(G_arr,   (N + 1) * sizeof(seq_t));

    /* --- Euler totient ------------------------------------- */
    for (size_t i = 0; i <= N; ++i) phi_arr[i] = i;
    if (N >= 1) phi_arr[1] = 1;
    for (size_t i = 2; i <= N; ++i)
        if (phi_arr[i] == i)                 /* i is prime */
            for (size_t j = i; j <= N; j += i)
                phi_arr[j] -= phi_arr[j] / i;

    /* --- T and G tables ------------------------------------ */
    T_arr[0] = (seq_t)1;
    G_arr[0] = (seq_t)0;
    if (N >= 1)
    {
        T_arr[1] = (seq_t)3;   /* {0,1,2} */
        G_arr[1] = (seq_t)2;   /* {1,2}  */

        /* DP states for T (last symbol 0 / non‑0) */
        seq_t Z = 1; /* strings of length 1 ending with 0   */
        seq_t Nstate = 2; /* strings of length 1 ending with non‑0 */

        /* DP states for G (strings starting with non‑0) */
        seq_t GZ = 0; /* last symbol 0 not allowed at start */
        seq_t GN = 2; /* first symbol non‑0, last non‑0 for length 1 */

        for (size_t len = 2; len <= N; ++len)
        {
            /*   T recurrence   */
            seq_t Z_next = Nstate;
            seq_t N_next = (Z + Nstate) * (seq_t)2;
            Z = Z_next;  Nstate = N_next;
            T_arr[len] = Z + Nstate;

            /*   G recurrence   */
            seq_t GZ_next = GN;
            seq_t GN_next = (GZ + GN) * (seq_t)2;
            GZ = GZ_next; GN = GN_next;
            G_arr[len] = GN;            /* last symbol non‑0 */
        }
    }

    val_size = N;
}

/* ----------  helper for B(d)  ---------------- */
static inline seq_t B_of(idx_t d)
{
    if (d == 1) return (seq_t)2;
    /* d ≥ 2  =>  d – 2 ≥ 0  so G[d‑2] is defined  */
    return T_arr[d] - G_arr[d - 2];
}

/* ----------  main sequence function  --------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n = 0, safety */
    init_up_to(n);

    seq_t sum = 0;
    for (idx_t d = 1; (idx_t)d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t div1 = n / d;
            seq_t term1 = (seq_t)phi_arr[div1] * B_of(d);
            sum += term1;

            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t term2 = (seq_t)phi_arr[n / d2] * B_of(d2);
                sum += term2;
            }
        }
    }
    return sum / (seq_t)n;
}
