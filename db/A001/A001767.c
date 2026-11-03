/*
 * A001767  Genus of modular group Gamma(n) = genus of modular curve Chi(n).
 * https://oeis.org/A001767
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (n-6)*A000010(n)*A001615(n)/24, for n > 2; a(2) = 0
 * Range:     u8 n <= 22, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 951 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

/* Computes phi(n) and psi(n) (Dedekind psi) by trial division */
static void a1767_phi_psi(uint64_t n, uint64_t *phi, uint64_t *psi) {
	uint64_t ph = n, ps = n, m = n;
	uint64_t p;
	for (p = 2; p * p <= m; p++) {
		if (m % p == 0) {
			ph = ph / p * (p - 1);
			ps = ps / p * (p + 1);
			while (m % p == 0) m /= p;
		}
	}
	if (m > 1) {
		ph = ph / m * (m - 1);
		ps = ps / m * (m + 1);
	}
	*phi = ph;
	*psi = ps;
}

seq_t a(idx_t n) {
	uint64_t phi, psi, P, g, d, m;
	seq_t r;
	if (n < 6) return (seq_t)0;
	a1767_phi_psi((uint64_t)n, &phi, &psi);
	P = phi * psi;
	m = (uint64_t)n - 6;
	g = gcd_u64(P, 24);
	P /= g;
	d = 24 / g;   /* coprime to P/g, hence divides m */
	m /= d;
	r = (seq_t)m * (seq_t)P;
	return r + 1;
}
