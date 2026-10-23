/*
 * A167284  A triangular sequence related to the EulerPhi function:
 *          t(n,k)=If[Mod[k, n] == 0 && (Mod[k, EulerPhi[n]] == 0), 1, Mod[k,
 *          n] ^ Mod[k, EulerPhi[n]]].
 * https://oeis.org/A167284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   phi_idx_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/******************************************************
 * Sequence A167284:  a(n) = t(r,k)
 *
 * t(n,k) = If[ Mod[k,n] == 0  &&  Mod[k, EulerPhi[n]] == 0,
 *              1,
 *              Mod[k,n] ^ Mod[k, EulerPhi[n]] ]
 *
 * r = the smallest integer with r*(r+1)/2 >= n
 * k = n - r*(r-1)/2      (1‑based column index within row r)
 *
 * This implementation directly evaluates the defining formula
 * for any n ≥ 1.  The function is fully memo‑free and works
 * for seq_t of any size supported by the build.
 ******************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"
#include "../utils/phi_idx_u64.h"

/* Main function for the OEIS sequence.               */
seq_t a(idx_t n) {
    /* Determine row r such that r*(r+1)/2 ≥ n.          */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Column index within the row.                     */
    idx_t k = n - (r * (r - 1)) / 2;   /* 1 ≤ k ≤ r */

    /* Compute Mod[k,n] and Mod[k,φ(n)].                 */
    seq_t mod_kn  = (seq_t)(k % r);
    idx_t phi_r   = phi_idx_u64(r);
    seq_t mod_kphi = (seq_t)(k % phi_r);

    /* Apply the defining rule of t(n,k).              */
    if (mod_kn == 0 && mod_kphi == 0) return (seq_t)1;

    /* Power computation: base^(exp).                   */
    return pow_uint((uint64_t)mod_kn, (idx_t)mod_kphi);
}
